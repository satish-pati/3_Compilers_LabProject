import re
import sys

_var_types = {}

def set_type(var, typ):
    _var_types[var] = typ

def get_type(var):
    return _var_types.get(var, 'int')

def is_long_type(var):
    return get_type(var) == 'long'

def is_short_type(var):
    return get_type(var) == 'short'

def is_char_type(var):
    return get_type(var) == 'char'

def is_float_type(var):
    return get_type(var) in ('float', 'double')

def is_double_type(var):
    return get_type(var) == 'double'

def load_ins(var):
    t = get_type(var)
    if t == 'long':   return 'ld'
    if t == 'short':  return 'lh'
    if t == 'char':   return 'lb'
    return 'lw'

def store_ins(var):
    t = get_type(var)
    if t == 'long':   return 'sd'
    if t == 'short':  return 'sh'
    if t == 'char':   return 'sb'
    return 'sw'

def print_fmt(var):
    t = get_type(var)
    if t == 'long':            return '.fmt_long'
    if t in ('float','double'): return '.fmt_float'
    if t == 'char':             return '.fmt_char'
    return '.fmt_int'

def scan_fmt(var):
    t = get_type(var)
    if t == 'long':             return '.fmt_scan_long'
    if t in ('float','double'): return '.fmt_scan_float'
    return '.fmt_scan_int'

def is_int_literal(v):
    try:
        int(v)
        return True
    except (ValueError, TypeError):
        return False

def is_num_literal(v):
    try:
        float(v)
        return True
    except (ValueError, TypeError):
        return False

def to_int(v):
    return int(float(v))

def parse_line(raw):
    raw = raw.strip()
    m = re.match(r'^(\d+)\s+(.*)', raw)
    return (int(m.group(1)), m.group(2).strip()) if m else (None, raw)

def _extract_defs_uses(instr):
    KEYWORDS = {'BeginFunc','EndFunc','PopParam','PushParam','Return',
                'Call','goto','if','int','float','char','string','double',
                'bool','long','short','unsigned','uint','byte','word',
                'printint','printfloat','printchar','printstring',
                'inputint','inputfloat','inputchar','inputstring',
                'rtz','main','eq','ne','lt','le','gt','ge','true','false'}

    defs, uses = set(), set()

    def is_var(t):
        return bool(re.match(r'^[A-Za-z_]\w*$', t)) and t not in KEYWORDS

    if re.match(r'^(BeginFunc|EndFunc)\b', instr):
        return defs, uses

    m = re.match(r'^PopParam\s+(\w+)$', instr)
    if m:
        defs.add(m.group(1)); return defs, uses

    m = re.match(r'^Return\s+(\S+)$', instr)
    if m:
        tok = m.group(1)
        if is_var(tok): uses.add(tok)
        return defs, uses

    m = re.match(r'^PushParam\s+(\S+)$', instr)
    if m:
        tok = m.group(1)
        if is_var(tok): uses.add(tok)
        return defs, uses

    m = re.match(r'^(\w+)\s*=\s*Call\s+\w+$', instr)
    if m:
        defs.add(m.group(1)); return defs, uses

    if re.match(r'^Call\s+\w+$', instr):
        return defs, uses

    m = re.match(r'^(?:print|input)\w+\s+(\S+)$', instr)
    if m:
        tok = m.group(1)
        if is_var(tok): uses.add(tok)
        return defs, uses

    m = re.match(r'^if\s+(\S+)\s+\S+\s+(\S+)\s+goto\s+\d+$', instr)
    if m:
        for tok in (m.group(1), m.group(2)):
            if is_var(tok):
                uses.add(tok)
            else:
                am = re.match(r'^(\w+)\[(\w+)\]$', tok)
                if am:
                    if is_var(am.group(1)): uses.add(am.group(1))
                    if is_var(am.group(2)): uses.add(am.group(2))
        return defs, uses

    if re.match(r'^goto\s+\d+$', instr):
        return defs, uses

    m = re.match(r'^(\w+)\s*=\s*(\S+)\s*(<=|>=|==|!=|<<|>>|[+\-*/%&|^<>])\s*(\S+)$', instr)
    if m:
        defs.add(m.group(1))
        for tok in (m.group(2), m.group(4)):
            if is_var(tok): uses.add(tok)
        return defs, uses

    m = re.match(r'^(\w+)\s*=\s*(\w+)\[(\w+)\]$', instr)
    if m:
        defs.add(m.group(1))
        for tok in (m.group(2), m.group(3)):
            if is_var(tok): uses.add(tok)
        return defs, uses

    m = re.match(r'^(\w+)\[(\w+)\]\s*=\s*(\S+)$', instr)
    if m:
        for tok in (m.group(1), m.group(2), m.group(3)):
            if is_var(tok): uses.add(tok)
        return defs, uses

    m = re.match(r'^(\w+)\s*=\s*\(\w+\)\s*(\S+)$', instr)
    if m:
        defs.add(m.group(1))
        tok = m.group(2)
        if is_var(tok): uses.add(tok)
        return defs, uses

    m = re.match(r'^(\w+)\s*=\s*(\S+)$', instr)
    if m:
        defs.add(m.group(1))
        tok = m.group(2)
        if is_var(tok): uses.add(tok)
        return defs, uses

    return defs, uses

def _liveness_analysis(instrs):
    n = len(instrs)
    if n == 0:
        return [], []

    lnum_to_idx = {lnum: i for i, (lnum, _) in enumerate(instrs)}
    succ = [[] for _ in range(n)]
    for i, (lnum, instr) in enumerate(instrs):
        if i + 1 < n:
            succ[i].append(i + 1)
        for m in re.finditer(r'goto\s+(\d+)', instr):
            tgt = int(m.group(1))
            if tgt in lnum_to_idx:
                succ[i].append(lnum_to_idx[tgt])
        if re.match(r'^goto\s+\d+$', instr.strip()):
            tgt = int(re.search(r'\d+', instr).group())
            succ[i] = [lnum_to_idx[tgt]] if tgt in lnum_to_idx else []

    defs_list  = []
    uses_list  = []
    for _, instr in instrs:
        d, u = _extract_defs_uses(instr)
        defs_list.append(d)
        uses_list.append(u)

    live_in  = [set() for _ in range(n)]
    live_out = [set() for _ in range(n)]

    changed = True
    while changed:
        changed = False
        for i in range(n - 1, -1, -1):
            new_out = set()
            for s in succ[i]:
                new_out |= live_in[s]
            new_in  = uses_list[i] | (new_out - defs_list[i])
            if new_in != live_in[i] or new_out != live_out[i]:
                live_in[i]  = new_in
                live_out[i] = new_out
                changed = True

    return live_in, live_out

def _compute_live_across_calls(instrs):
    if not instrs:
        return set()

    live_in, live_out = _liveness_analysis(instrs)

    def _is_call_site(instr):
        s = instr.strip()
        return (re.match(r'^(print|input)\w+', s) or
                re.match(r'^\w+\s*=\s*Call\b', s) or
                re.match(r'^Call\b', s))

    live_across = set()
    for i, (_, instr) in enumerate(instrs):
        if _is_call_site(instr):
            live_across |= live_out[i]
    return live_across

class RegAlloc:

    SAVED_POOL  = ["s1","s2","s3","s4","s5",
                   "s6","s7","s8","s9","s10","s11"]

    CALLER_POOL = ["t0","t1","t2","t3"]

    DEAD_POOL   = ["t4","t5","t6"]

    FP_CALLER_POOL = ["ft0","ft1","ft2","ft3","ft4","ft5","ft6","ft7",
                      "ft8","ft9","ft10","ft11"]

    FP_SAVED_POOL  = ["fs0","fs1","fs2","fs3","fs4","fs5","fs6","fs7",
                      "fs8","fs9","fs10","fs11"]

    K              = len(SAVED_POOL) + len(CALLER_POOL) + len(DEAD_POOL)
    SREG_SAVE_BASE = 16
    SREG_SLOTS     = 11

    TEMP_SCRATCH_MAP = {
        "t0": "t0", "t1": "t1", "t2": "t2", "t3": "t3",
    }

    def __init__(self):
        self._map         = {}
        self._spill       = {}
        self.spill_slots  = 0
        self.used_sregs   = []
        self.used_fp_sregs = []
        self._float_vars  = set()
        self._array_names = set()
        self._safe_scratch     = set()
        self._no_call_crossing = set()
        self._fp_no_call_crossing = set()
        self._spill_base  = 16 + 11 * 8

    def set_safe_scratch_temps(self, instrs):
        if instrs and isinstance(instrs[0], str):
            pairs = list(enumerate(instrs))
        else:
            pairs = list(instrs)

        if not pairs:
            self._safe_scratch     = set(self.TEMP_SCRATCH_MAP.keys())
            self._no_call_crossing = set()
            return

        all_vars = set()
        for _, instr in pairs:
            d, u = _extract_defs_uses(instr)
            all_vars |= d | u

        live_across = _compute_live_across_calls(pairs)

        safe_from_calls = all_vars - live_across

        all_tac_temps = set(self.TEMP_SCRATCH_MAP.keys())
        self._safe_scratch = (safe_from_calls & all_tac_temps) - {
            v for v in all_tac_temps if get_type(v) in ('float', 'double')
        }

        self._no_call_crossing = {
            v for v in safe_from_calls
            if not re.match(r'^t\d+$', v)
            and get_type(v) not in ('float','double')
            and v not in self._array_names
        }

        self._fp_no_call_crossing = {
            v for v in safe_from_calls
            if get_type(v) in ('float', 'double')
            and v not in self._array_names
        }

    @staticmethod
    def _compute_dead_after_def(instrs):
        if not instrs:
            return set()

        live_in, live_out = _liveness_analysis(instrs)

        def_site = {}
        for i, (_, instr) in enumerate(instrs):
            defs, _ = _extract_defs_uses(instr)
            for v in defs:
                def_site[v] = i

        used_anywhere = set()
        for _, instr in instrs:
            _, uses = _extract_defs_uses(instr)
            used_anywhere |= uses

        dead_after_def = set()
        for v, i in def_site.items():
            if v not in used_anywhere or v not in live_out[i]:
                dead_after_def.add(v)

        return dead_after_def

    def run_graph_coloring(self, instrs):
        if not instrs:
            return

        live_in, live_out = _liveness_analysis(instrs)

        dead_after_def = self._compute_dead_after_def(instrs)

        all_vars = set()
        for _, instr in instrs:
            d, u = _extract_defs_uses(instr)
            all_vars |= d | u

        forced_spill = set()
        for v in all_vars:
            is_tac_temp  = bool(re.match(r'^t\d+$', v))
            is_fp_var    = get_type(v) in ('float', 'double') and v not in self._array_names
            in_safe_scratch   = v in self._safe_scratch
            in_fp_no_crossing = v in self._fp_no_call_crossing
            in_fp_saved_ok    = is_fp_var and not in_fp_no_crossing

            if is_tac_temp and not in_safe_scratch and not is_fp_var:
                pass
            elif is_tac_temp and is_fp_var and not in_fp_no_crossing:
                forced_spill.add(v)
            elif is_fp_var and not is_tac_temp and not in_fp_no_crossing and not in_fp_saved_ok:
                pass
        self._float_vars = {v for v in forced_spill
                            if get_type(v) in ('float', 'double')}

        colorable = sorted(all_vars - forced_spill)

        colorable_set = set(colorable)
        adj = {v: set() for v in colorable}

        def add_edge(a, b):
            if a != b and a in adj and b in adj:
                adj[a].add(b)
                adj[b].add(a)

        for i, (_, instr) in enumerate(instrs):
            live_out_i = live_out[i] & colorable_set
            defs, _    = _extract_defs_uses(instr)

            copy_m = re.match(r'\A(\w+)\s*=\s*([A-Za-z_]\w*)\s*\Z', instr)
            copy_dst = copy_m.group(1) if copy_m else None
            copy_src = copy_m.group(2) if copy_m else None

            for d in (defs & colorable_set):
                for v in live_out_i:
                    if d == copy_dst and v == copy_src:
                        continue
                    add_edge(d, v)

            lv = list(live_out_i)
            for ii in range(len(lv)):
                for jj in range(ii + 1, len(lv)):
                    add_edge(lv[ii], lv[jj])

            live_in_i = live_in[i] & colorable_set
            li2 = list(live_in_i)
            for ii in range(len(li2)):
                for jj in range(ii + 1, len(li2)):
                    a, b = li2[ii], li2[jj]
                    if copy_dst and copy_src:
                        if (a == copy_dst and b == copy_src) or \
                           (a == copy_src and b == copy_dst):
                            continue
                    add_edge(a, b)

        for arr_var in self._array_names:
            if arr_var in colorable_set:
                for other in colorable_set:
                    add_edge(arr_var, other)

        stack            = []
        optimistic_spill = set()
        remaining        = set(colorable)
        def _sorted_remaining():
            return sorted(remaining, key=lambda v: (len(adj[v] & remaining), v))

        last_use = {}
        for i, (_, instr) in enumerate(instrs):
            _, uses = _extract_defs_uses(instr)
            for v in uses:
                last_use[v] = i

        def _spill_score(v):
            if v not in last_use:
                return (float('inf'), len(adj[v] & remaining), v)
            return (last_use[v], len(adj[v] & remaining), v)

        while remaining:
            candidate = next(
                (v for v in _sorted_remaining() if len(adj[v] & remaining) < self.K),
                None
            )
            if candidate is None:
                candidate = max(_sorted_remaining(), key=_spill_score)
                optimistic_spill.add(candidate)
            remaining.remove(candidate)
            stack.append((candidate, adj[candidate] & remaining))

        color   = {}
        spilled = set()

        while stack:
            v, neighbors = stack.pop()
            used = {color[n] for n in adj[v] if n in color}

            is_array_base    = (v in self._array_names)
            is_tac_safe_temp = (v in self._safe_scratch)

            if not is_tac_safe_temp:
                for n in adj[v]:
                    if n in self._safe_scratch and n in self.TEMP_SCRATCH_MAP:
                        used.add(self.TEMP_SCRATCH_MAP[n])
            crosses_call     = (v not in self._no_call_crossing
                                and not is_tac_safe_temp)

            if is_tac_safe_temp:
                color[v] = self.TEMP_SCRATCH_MAP.get(v, v)
                continue

            is_fp_var = (get_type(v) in ('float', 'double') and v not in self._array_names)

            if is_fp_var:
                if v in self._fp_no_call_crossing:
                    assigned = next(
                        (r for r in self.FP_CALLER_POOL if r not in used), None)
                    if assigned is None:
                        assigned = next(
                            (r for r in self.FP_SAVED_POOL if r not in used), None)
                else:
                    assigned = next(
                        (r for r in self.FP_SAVED_POOL if r not in used), None)
                if assigned:
                    color[v] = assigned
                else:
                    spilled.add(v)
                continue

            if crosses_call or is_array_base:
                assigned = next(
                    (r for r in self.SAVED_POOL if r not in used), None)
            else:
                assigned = next(
                    (r for r in self.CALLER_POOL if r not in used), None)
                if assigned is None and v in dead_after_def:
                    assigned = next(
                        (r for r in self.DEAD_POOL if r not in used), None)
                if assigned is None:
                    assigned = next(
                        (r for r in self.SAVED_POOL if r not in used), None)

            if assigned:
                color[v] = assigned
            else:
                spilled.add(v)

        used_regs_set = set()
        used_fp_sregs_set = set()
        for v, reg in color.items():
            if v not in spilled:
                self._map[v] = reg
                if reg in self.SAVED_POOL:
                    used_regs_set.add(reg)
                elif reg in self.FP_SAVED_POOL:
                    used_fp_sregs_set.add(reg)
                if get_type(v) in ('float', 'double') and v not in spilled:
                    self._float_vars.discard(v)

        for v in all_vars:
            if v in self._safe_scratch and v in self.TEMP_SCRATCH_MAP:
                self._map[v] = self.TEMP_SCRATCH_MAP[v]

        self.used_sregs = [r for r in self.SAVED_POOL if r in used_regs_set]
        self.used_fp_sregs = [r for r in self.FP_SAVED_POOL if r in used_fp_sregs_set]

        self._recompute_spill_base()

        for v in sorted(forced_spill):
            self.spill_slots += 1
            self._spill[v] = -(self._spill_base + self.spill_slots * 8)

        for v in sorted(spilled):
            if v not in self._spill:
                self.spill_slots += 1
                self._spill[v] = -(self._spill_base + self.spill_slots * 8)

    def _recompute_spill_base(self):
        header = 16
        self._spill_base = header + len(self.used_sregs) * 8 + len(self.used_fp_sregs) * 8

    def _alloc(self, var):
        if var in self._safe_scratch and var in self.TEMP_SCRATCH_MAP:
            hw = self.TEMP_SCRATCH_MAP[var]
            self._map[var] = hw
            return hw

        if re.match(r'^t\d+$', var) and var not in self._safe_scratch:
            self.spill_slots += 1
            off = -(self._spill_base + self.spill_slots * 8)
            self._spill[var] = off
            return None

        if (get_type(var) in ('float', 'double') and var not in self._array_names):
            used = set(self._map.values())
            if var in self._fp_no_call_crossing:
                for reg in self.FP_CALLER_POOL:
                    if reg not in used:
                        self._map[var] = reg
                        return reg
                for reg in self.FP_SAVED_POOL:
                    if reg not in used:
                        self._map[var] = reg
                        if reg not in self.used_fp_sregs:
                            self.used_fp_sregs.append(reg)
                        return reg
            else:
                for reg in self.FP_SAVED_POOL:
                    if reg not in used:
                        self._map[var] = reg
                        if reg not in self.used_fp_sregs:
                            self.used_fp_sregs.append(reg)
                        return reg
            self.spill_slots += 1
            off = -(self._spill_base + self.spill_slots * 8)
            self._spill[var] = off
            if get_type(var) in ('float', 'double'):
                self._float_vars.add(var)
            return None

        used = set(self._map.values())

        is_array_base = (var in self._array_names)
        crosses_call  = (var not in self._no_call_crossing)

        if not re.match(r'^t\d+$', var) and not crosses_call and not is_array_base:
            for tac_tmp, hw in self.TEMP_SCRATCH_MAP.items():
                if tac_tmp in self._safe_scratch:
                    used.add(hw)

        if crosses_call or is_array_base:
            for reg in self.SAVED_POOL:
                if reg not in used:
                    self._map[var] = reg
                    if reg not in self.used_sregs:
                        self.used_sregs.append(reg)
                    return reg
        else:
            for reg in self.CALLER_POOL:
                if reg not in used:
                    self._map[var] = reg
                    return reg
            for reg in self.SAVED_POOL:
                if reg not in used:
                    self._map[var] = reg
                    if reg not in self.used_sregs:
                        self.used_sregs.append(reg)
                    return reg

        self.spill_slots += 1
        off = -(self._spill_base + self.spill_slots * 8)
        self._spill[var] = off
        return None

    def reg(self, var):
        if var not in self._map and var not in self._spill:
            self._alloc(var)
        return self._map.get(var)

    def is_spilled(self, var):
        if var not in self._map and var not in self._spill:
            self._alloc(var)
        return var in self._spill

    def spill_offset(self, var):
        return self._spill[var]

    def ensure(self, var):
        if var not in self._map and var not in self._spill:
            self._alloc(var)

    def force_spill(self, var):
        if var in self._spill:
            return self._spill[var]
        if var in self._map:
            freed = self._map.pop(var)
            if freed in self.used_sregs:
                self.used_sregs.remove(freed)
        self.spill_slots += 1
        off = -(self._spill_base + self.spill_slots * 8)
        self._spill[var] = off
        return off

def _copy_propagate(tac):
    KEYWORDS = {'BeginFunc','EndFunc','PopParam','PushParam','Return',
                'Call','goto','if','int','float','char','string','double',
                'bool','long','short','unsigned','uint','byte','word',
                'printint','printfloat','printchar','printstring',
                'inputint','inputfloat','inputchar','inputstring',
                'rtz','main','eq','ne','lt','le','gt','ge','true','false'}

    def _is_plain_var(tok):
        return bool(re.match(r'^[A-Za-z_]\w*$', tok)) and tok not in KEYWORDS

    changed = True
    while changed:
        changed = False
        lnums  = [lnum  for lnum, _ in tac]
        instrs = [instr for _, instr in tac]
        n      = len(instrs)

        jump_targets = set()
        for instr in instrs:
            for m in re.finditer(r'goto\s+(\d+)', instr):
                jump_targets.add(int(m.group(1)))

        remove      = set()
        new_instrs  = list(instrs)

        for i in reversed(range(n)):
            instr = new_instrs[i]
            m = re.match(r'^(\w+)\s*=\s*([A-Za-z_]\w*)\s*$', instr)
            if not m:
                continue
            dst, src = m.group(1), m.group(2)
            if not (_is_plain_var(dst) and _is_plain_var(src)):
                continue
            if dst == src:
                if lnums[i] not in jump_targets:
                    remove.add(i)
                    changed = True
                continue
            if lnums[i] in jump_targets:
                continue

            dst_uses      = []
            src_kill_idx  = None
            has_back_edge = False

            dst_redefined_after = False
            dst_redef_idx = None
            for j in range(i + 1, n):
                defs_j, uses_j = _extract_defs_uses(new_instrs[j])

                if dst in defs_j:
                    dst_redefined_after = True
                    dst_redef_idx = j
                    break

                if src_kill_idx is None and src in defs_j:
                    src_kill_idx = j

                if dst in uses_j:
                    dst_uses.append(j)

                for bm in re.finditer(r'goto\s+(\d+)', new_instrs[j]):
                    tgt = int(bm.group(1))
                    if tgt <= lnums[i]:
                        has_back_edge = True

            if has_back_edge:
                continue

            if dst_redefined_after and dst_uses:
                last_use_lnum = lnums[dst_uses[-1]]
                redef_lnum    = lnums[dst_redef_idx]
                unsafe = False
                for k in range(i, n):
                    for bm in re.finditer(r'goto\s+(\d+)', new_instrs[k]):
                        tgt_lnum = int(bm.group(1))
                        if lnums[i] < tgt_lnum <= last_use_lnum:
                            unsafe = True
                            break
                        if tgt_lnum <= lnums[i]:
                            unsafe = True
                            break
                        if k < dst_redef_idx and tgt_lnum > redef_lnum:
                            unsafe = True
                            break
                    if unsafe:
                        break
                if unsafe:
                    continue

            if not dst_uses:
                remove.add(i)
                changed = True
                continue

            if src_kill_idx is not None and src_kill_idx <= max(dst_uses):
                continue

            src_is_temp = bool(re.match(r'^t\d+$', src))
            safe_uses = []
            unsafe_found = False
            for j in dst_uses:
                instr_j = new_instrs[j]
                if src_is_temp:
                    arr_write_m = re.match(r'^(\w+)\[(\w+)\]\s*=\s*(.+)$', instr_j)
                    if arr_write_m and arr_write_m.group(2) == dst:
                        unsafe_found = True
                        break
                safe_uses.append(j)

            if unsafe_found:
                continue

            for j in safe_uses:
                old = new_instrs[j]
                new_instrs[j] = re.sub(r'\b' + re.escape(dst) + r'\b', src, old)
                if new_instrs[j] != old:
                    changed = True

            remove.add(i)
            changed = True

        tac = [(lnum, instr)
               for i, (lnum, instr) in enumerate(zip(lnums, new_instrs))
               if i not in remove]

    return tac

FRAME = 512

def _compute_frame(n_sregs, n_spill_slots, needs_ra, needs_fp):
    header = (8 if needs_ra else 0) + (8 if needs_fp else 0)
    actual_spill_base = header + n_sregs * 8

    size = actual_spill_base + n_spill_slots * 8
    if size % 16:
        size += 16 - (size % 16)
    return max(size, 16)

class TACtoRISCV:

    BRANCH_MAP = {
        '==': ("beq", False), 'eq': ("beq", False),
        '!=': ("bne", False), 'ne': ("bne", False),
        '<' : ("blt", False), 'lt': ("blt", False),
        '>=': ("bge", False), 'ge': ("bge", False),
        '>' : ("blt", True),  'gt': ("blt", True),
        '<=': ("bge", True),  'le': ("bge", True),
    }

    BINOP_MAP = {
        '+' : ("add",  "addi"),
        '-' : ("sub",  None),
        '*' : ("mul",  None),
        '/' : ("div",  None),
        '%' : ("rem",  None),
        '&' : ("and",  "andi"),
        '|' : ("or",   "ori"),
        '^' : ("xor",  "xori"),
        '<<': ("sll",  "slli"),
        '>>': ("sra",  "srai"),
    }

    def __init__(self):
        self.out          = []
        self.tac          = []
        self.jump_targets = set()
        self.tac_line_set = set()
        self.str_lits     = []
        self.flt_lits     = []

        self._used_externs = set()
        self._used_fmts    = set()

        self.fname            = ""
        self.ra_alloc         = RegAlloc()
        self.param_q          = []
        self.pop_idx = 0
        self.int_pop_idx = 0
        self.float_pop_idx = 0
        self.epilogue_lbl     = ""
        self.in_main          = False
        self._fn_tac_lines    = []

        self._fn_targets  = set()
        self._fn_lines    = set()

        self._global_targets = set()
        self._global_lines   = set()
        self._in_function    = False

        self._arrays      = {}
        self._array_order = []

    def e(self, line=""):
        self.out.append(line)

    def lbl(self, name):
        self.out.append(f"{name}:")

    def ins(self, op, *args):
        arg = ", ".join(str(a) for a in args)
        self.out.append(f"    {op:<8} {arg}".rstrip())

    def _is_fp_reg(self, var):
        r = self.ra_alloc._map.get(var)
        if r and (r in RegAlloc.FP_CALLER_POOL or r in RegAlloc.FP_SAVED_POOL):
            return r
        return None

    def _load_float_var_into(self, var, fa_reg):
        fp_r = self._is_fp_reg(var)
        if fp_r:
            if fp_r != fa_reg:
                self.ins("fmv.d", fa_reg, fp_r)
        elif self.ra_alloc.is_spilled(var):
            self.ins("fld", fa_reg, f"{self.ra_alloc.spill_offset(var)}(s0)")
        else:
            r = self.ra_alloc.reg(var)
            self.ins("fcvt.d.w", fa_reg, r if r else "zero")

    def _store_float_result(self, fa_reg, dst_var):
        fp_r = self._is_fp_reg(dst_var)
        if fp_r:
            if fp_r != fa_reg:
                self.ins("fmv.d", fp_r, fa_reg)
        else:
            off = self.ra_alloc.force_spill(dst_var)
            self.ins("fsd", fa_reg, f"{off}(s0)")

    def use_extern(self, sym):
        self._used_externs.add(sym)

    def use_fmt(self, fmt):
        self._used_fmts.add(fmt)

    def _fn_has_calls(self, instrs):
        for s in instrs:
            if re.search(r'\bCall\b', s):          return True
            if re.match(r'^print', s.strip()):    return True
            if re.match(r'^input', s.strip()):    return True
        return False

    def _fn_needs_fp(self, ra_alloc):
        return bool(ra_alloc.spill_slots)

    def load(self, val, dst):
        if is_int_literal(val):
            self.ins("li", dst, to_int(val))
        elif is_num_literal(val):
            lname = f".flt{len(self.flt_lits)}"
            self.flt_lits.append((lname, val))
            self.ins("la",  "t6", lname)
            self.ins("fld", dst, f"0(t6)")
        elif self.ra_alloc.is_spilled(val):
            ins = load_ins(val)
            self.ins(ins, dst, f"{self.ra_alloc.spill_offset(val)}(s0)")
        else:
            r = self.ra_alloc.reg(val)
            if r and r != dst:
                self.ins("mv", dst, r)

    def store_var(self, var, src):
        if self.ra_alloc.is_spilled(var):
            ins = store_ins(var)
            self.ins(ins, src, f"{self.ra_alloc.spill_offset(var)}(s0)")
        else:
            r = self.ra_alloc.reg(var)
            if r and r != src:
                self.ins("mv", r, src)

    def var_reg_or_load(self, var, scratch):
        if is_int_literal(var):
            self.ins("li", scratch, to_int(var))
            return scratch
        if self.ra_alloc.is_spilled(var):
            ins = load_ins(var)
            self.ins(ins, scratch, f"{self.ra_alloc.spill_offset(var)}(s0)")
            return scratch
        r = self.ra_alloc.reg(var)
        return r if r else scratch

    def dst_reg(self, var):
        if self.ra_alloc.is_spilled(var):
            return "t4"
        r = self.ra_alloc.reg(var)
        return r if r else "t4"

    def flush_dst(self, var, reg):
        if self.ra_alloc.is_spilled(var):
            ins = store_ins(var)
            self.ins(ins, reg, f"{self.ra_alloc.spill_offset(var)}(s0)")

    def operand(self, val, scratch):
        if is_int_literal(val):
            self.ins("li", scratch, to_int(val))
            return scratch
        if self.ra_alloc.is_spilled(val):
            self.ins(load_ins(val), scratch, f"{self.ra_alloc.spill_offset(val)}(s0)")
            return scratch
        r = self.ra_alloc.reg(val)
        return r if r else scratch

    def tac_lbl(self, n):
        return f".L{n}"

    def collect_targets(self):
        in_fn = False
        fn_targets = set()
        fn_lines   = set()
        self._fn_scope_list = []

        for lnum, instr in self.tac:
            self.tac_line_set.add(lnum)
            for m in re.finditer(r'goto\s+(\d+)', instr):
                self.jump_targets.add(int(m.group(1)))

            if re.match(r'^BeginFunc', instr):
                in_fn      = True
                fn_targets = set()
                fn_lines   = set()
                fn_lines.add(lnum)
            elif re.match(r'^EndFunc', instr):
                fn_lines.add(lnum)
                self._fn_scope_list.append((fn_targets, fn_lines))
                in_fn = False
            else:
                if in_fn:
                    fn_lines.add(lnum)
                    for m in re.finditer(r'goto\s+(\d+)', instr):
                        fn_targets.add(int(m.group(1)))
                else:
                    self._global_lines.add(lnum)
                    for m in re.finditer(r'goto\s+(\d+)', instr):
                        self._global_targets.add(int(m.group(1)))

        self._fn_scope_iter = iter(self._fn_scope_list)

        self._array_stride = {}
        seen_order = []
        arr_offsets = {}
        for _, instr in self.tac:
            for m in re.finditer(r'\b([A-Za-z_]\w*)\[(\d+)\]', instr):
                aname, byte_off = m.group(1), int(m.group(2))
                if aname not in self._arrays:
                    self._arrays[aname] = byte_off
                    seen_order.append(aname)
                    arr_offsets[aname] = set()
                else:
                    self._arrays[aname] = max(self._arrays[aname], byte_off)
                arr_offsets[aname].add(byte_off)
        for aname, offsets in arr_offsets.items():
            nonzero = sorted(o for o in offsets if o > 0)
            if nonzero:
                stride = nonzero[0]
            else:
                stride = 4
            self._array_stride[aname] = stride
            if stride == 1:
                set_type(aname, 'char')
            elif stride == 2:
                set_type(aname, 'short')
            elif stride == 8:
                set_type(aname, 'double')

        for _, instr in self.tac:
            m = re.match(r'^(\w+)\[\d+\]\s*=\s*(\S+)$', instr)
            if m:
                aname, val = m.group(1), m.group(2)
                if aname in self._arrays and is_num_literal(val) and not is_int_literal(val):
                    set_type(aname, 'float')
        self._array_order = seen_order

    def emit_missing_target_stubs(self):
        try:
            fn_targets, fn_lines = next(self._fn_scope_iter)
        except StopIteration:
            return
        beginfunc_map = {}
        for lnum, instr in self.tac:
            m = re.match(r'^BeginFunc\s+(\w+)', instr)
            if m:
                beginfunc_map[lnum] = m.group(1)
        missing = fn_targets - fn_lines
        real_missing = missing - set(beginfunc_map.keys())
        fn_jumps_to_func = missing & set(beginfunc_map.keys())
        if fn_jumps_to_func:
            self.e()
            self.e(f"    # -- cross-function jump redirects --")
            for n in sorted(fn_jumps_to_func):
                self.lbl(self.tac_lbl(n))
                self.ins('j', beginfunc_map[n])
        if real_missing:
            self.e()
            self.e(f"    # -- stub labels for out-of-range jump targets --")
            for n in sorted(real_missing):
                self.lbl(self.tac_lbl(n))
                self.ins("nop")

    def _prescan_function(self, fn_tac_lines):
        KEYWORDS = {'BeginFunc','EndFunc','PopParam','PushParam','Return',
                    'Call','goto','if','int','float','char','string','double',
                    'bool','long','short','unsigned','uint','byte','word',
                    'printint','printfloat','printchar','printstring',
                    'inputint','inputfloat','inputchar','inputstring',
                    'rtz','main'}

        for instr in fn_tac_lines:
            instr = re.sub(r'^//.*', '', instr).strip()
            if not instr:
                continue
            if re.match(r'^(BeginFunc|EndFunc)\b', instr):
                continue

            m = re.match(r'^(\w+)\s*=\s*Call\s+\w+$', instr)
            if m:
                self.ra_alloc.ensure(m.group(1)); continue

            if re.match(r'^Call\s+\w+$', instr):
                continue

            m = re.match(r'^PushParam\s+(\S+)$', instr)
            if m:
                tok = m.group(1)
                if not is_num_literal(tok) and re.match(r'^[A-Za-z_]\w*$', tok) and tok not in KEYWORDS:
                    self.ra_alloc.ensure(tok)
                continue

            m = re.match(r'^PopParam\s+(\w+)$', instr)
            if m:
                self.ra_alloc.ensure(m.group(1)); continue

            m = re.match(r'^Return\s+(\S+)$', instr)
            if m:
                tok = m.group(1)
                if not is_num_literal(tok) and re.match(r'^[A-Za-z_]\w*$', tok) and tok not in KEYWORDS:
                    self.ra_alloc.ensure(tok)
                continue

            m = re.match(r'^(\w+)\s*=\s*\(\w+\)\s*(\S+)$', instr)
            if m:
                self.ra_alloc.ensure(m.group(1))
                src = m.group(2)
                if not is_num_literal(src) and not src.startswith('"') and \
                        re.match(r'^[A-Za-z_]\w*$', src) and src not in KEYWORDS:
                    self.ra_alloc.ensure(src)
                continue

            for tok in re.findall(r'\b([A-Za-z_]\w*)\b', instr):
                if tok not in KEYWORDS and not re.match(r'^t\d+$', tok):
                    self.ra_alloc.ensure(tok)
            for tok in re.findall(r'\bt\d+\b', instr):
                self.ra_alloc.ensure(tok)

    def _preseed_float_types(self, tac_lines):
        FLOAT_OPS = {'+', '-', '*', '/'}
        changed = True
        while changed:
            changed = False
            for instr in tac_lines:
                instr = instr.strip()

                m = re.match(r'^(\w+)\s*=\s*(\S+)$', instr)
                if m:
                    dst, val = m.group(1), m.group(2)
                    if is_num_literal(val) and not is_int_literal(val):
                        if get_type(dst) not in ('float', 'double'):
                            set_type(dst, 'float')
                            changed = True
                    elif re.match(r'^[A-Za-z_]\w*$', val) and get_type(val) in ('float', 'double'):
                        if get_type(dst) not in ('float', 'double'):
                            set_type(dst, 'float')
                            changed = True
                    continue

                m = re.match(r'^(\w+)\s*=\s*\((float|double)\)\s*(\S+)$', instr)
                if m:
                    dst, cast_type = m.group(1), m.group(2)
                    if get_type(dst) not in ('float', 'double'):
                        set_type(dst, cast_type)
                        changed = True
                    continue

                m = re.match(
                    r'^(\w+)\s*=\s*(.+?)\s*(<=|>=|==|!=|<<|>>|[+\-*/%&|^<>])\s*(.+)$',
                    instr)
                if m and m.group(2).strip():
                    dst  = m.group(1)
                    op1  = m.group(2).strip()
                    op   = m.group(3)
                    op2  = m.group(4).strip()
                    if op in FLOAT_OPS:
                        op1_float = (
                            (is_num_literal(op1) and not is_int_literal(op1)) or
                            (re.match(r'^[A-Za-z_]\w*$', op1) and get_type(op1) in ('float','double'))
                        )
                        op2_float = (
                            (is_num_literal(op2) and not is_int_literal(op2)) or
                            (re.match(r'^[A-Za-z_]\w*$', op2) and get_type(op2) in ('float','double'))
                        )
                        if (op1_float or op2_float) and get_type(dst) not in ('float','double'):
                            set_type(dst, 'float')
                            changed = True
                    continue

                m = re.match(r'^print(float|double)\s+(\w+)$', instr)
                if m and get_type(m.group(2)) not in ('float', 'double'):
                    set_type(m.group(2), m.group(1))
                    changed = True

    def prologue(self, fname, nparams, fn_tac_lines=None):
        self.fname        = fname
        self.in_main      = (fname == "main")
        self._in_function = True
        self.ra_alloc     = RegAlloc()
        self.ra_alloc._array_names = set(self._arrays.keys())
        self.param_q      = []
        self.pop_idx = 0
        self.int_pop_idx = 0
        self.float_pop_idx = 0
        self.epilogue_lbl = f".Lepilogue_{fname}"
        self._fn_tac_lines = fn_tac_lines or []

        if fn_tac_lines:
            self._preseed_float_types(fn_tac_lines)
            self.ra_alloc.set_safe_scratch_temps(fn_tac_lines)
            fn_pairs = list(enumerate(fn_tac_lines))
            self.ra_alloc.run_graph_coloring(fn_pairs)
            self._prescan_function(fn_tac_lines)

        sregs = list(self.ra_alloc.used_sregs)
        fp_sregs = list(self.ra_alloc.used_fp_sregs)
        self._prologue_sregs = sregs
        self._prologue_fp_sregs = fp_sregs

        fn_instrs_strs = fn_tac_lines or []
        needs_ra = self._fn_has_calls(fn_instrs_strs)
        needs_fp = self._fn_needs_fp(self.ra_alloc)

        if not needs_fp and fn_tac_lines:
            for var in self.ra_alloc._spill:
                if re.match(r'^t\d+$', var) and get_type(var) not in ('float', 'double'):
                    needs_fp = True
                    break

        float_reserve = 0
        if needs_fp and fn_tac_lines:
            fp_pool = set(RegAlloc.FP_CALLER_POOL) | set(RegAlloc.FP_SAVED_POOL)
            for line in fn_tac_lines:
                for tok in re.findall(r'\b[A-Za-z_]\w*\b', line):
                    if (get_type(tok) in ('float', 'double')
                            and tok not in self.ra_alloc._map
                            and tok not in self.ra_alloc._spill):
                        float_reserve += 1
            float_reserve = min(float_reserve, 8)

        frame = _compute_frame(len(sregs) + len(fp_sregs),
                               self.ra_alloc.spill_slots + float_reserve,
                               needs_ra, needs_fp)
        self._prologue_frame    = frame
        self._prologue_needs_ra = needs_ra
        self._prologue_needs_fp = needs_fp

        self.e()
        self.ins(".globl", fname)
        self.lbl(fname)
        self.e(f"    # -- prologue: {fname}  ({nparams} params) --")

        if frame > 0:
            self.ins("addi", "sp, sp", f"-{frame}")

        off = frame
        if needs_ra:
            off -= 8
            self.ins("sd", "ra", f"{off}(sp)")
        if needs_fp:
            off -= 8
            self.ins("sd", "s0", f"{off}(sp)")
        for reg in sregs:
            off -= 8
            self.ins("sd", reg, f"{off}(sp)")
        for reg in fp_sregs:
            off -= 8
            self.ins("fsd", reg, f"{off}(sp)")

        if needs_fp:
            self.ins("addi", "s0, sp", str(frame))

        if self._arrays:
            self.e(f"    # -- array base pointers --")
            self.emit_array_la()

    def epilogue(self):
        self.emit_missing_target_stubs()

        sregs    = self._prologue_sregs
        fp_sregs = getattr(self, '_prologue_fp_sregs', [])
        frame    = self._prologue_frame
        needs_ra = self._prologue_needs_ra
        needs_fp = self._prologue_needs_fp

        self.e()
        self.lbl(self.epilogue_lbl)
        self.e(f"    # -- epilogue: {self.fname} --")

        t_pool = set(self.ra_alloc.CALLER_POOL)

        live_at_exit = set()
        if self._fn_tac_lines:
            KEYWORDS_LIVE = {'BeginFunc','EndFunc','PopParam','PushParam','Return',
                             'Call','goto','if','int','float','char','string','double',
                             'bool','long','short','unsigned','uint','byte','word',
                             'printint','printfloat','printchar','printstring',
                             'inputint','inputfloat','inputchar','inputstring',
                             'rtz','main','eq','ne','lt','le','gt','ge','true','false'}
            pairs = list(enumerate(self._fn_tac_lines))
            _, live_out = _liveness_analysis(pairs)
            if live_out:
                live_at_exit = live_out[-1]

        off = frame
        if needs_ra:
            off -= 8
            ra_off = off
        if needs_fp:
            off -= 8
            s0_off = off
        sreg_offs = []
        for _ in sregs:
            off -= 8
            sreg_offs.append(off)
        fp_sreg_offs = []
        for _ in fp_sregs:
            off -= 8
            fp_sreg_offs.append(off)

        if needs_ra:
            self.ins("ld", "ra", f"{ra_off}(sp)")
        for reg, roff in zip(sregs, sreg_offs):
            self.ins("ld", reg, f"{roff}(sp)")
        for reg, roff in zip(fp_sregs, fp_sreg_offs):
            self.ins("fld", reg, f"{roff}(sp)")
        if needs_fp:
            self.ins("ld", "s0", f"{s0_off}(sp)")

        if frame > 0:
            self.ins("addi", "sp, sp", str(frame))
        self.ins("ret")

    def emit_branch(self, lhs, op, rhs, label):
        if re.match(r'^(\w+)\[(\w+)\]$', lhs):
            self._resolve_operand(lhs, "t4")
            lhs_reg = "t4"
        else:
            lhs_reg = self.var_reg_or_load(lhs, "t4")

        if re.match(r'^(\w+)\[(\w+)\]$', rhs):
            self._resolve_operand(rhs, "t5")
            rhs_reg = "t5"
        else:
            rhs_reg = self.var_reg_or_load(rhs, "t5")
            if lhs_reg == rhs_reg and lhs_reg not in ("t4", "t5"):
                self.load(rhs, "t5")
                rhs_reg = "t5"

        br, swap = self.BRANCH_MAP.get(op, ("bne", False))
        if swap:
            self.ins(br, f"{rhs_reg}, {lhs_reg}, {label}")
        else:
            self.ins(br, f"{lhs_reg}, {rhs_reg}, {label}")

    def emit_binary(self, dst_var, op1, op, op2):
        rr, ri = self.BINOP_MAP.get(op, (None, None))
        if not rr:
            self.e(f"    # [UNSUPPORTED OP '{op}']")
            return

        d = self.dst_reg(dst_var)
        if re.match(r'^(\w+)\[(\w+)\]$', op1):
            self._resolve_operand(op1, "t4")
            op1_reg = "t4"
        else:
            op1_reg = self.var_reg_or_load(op1, "t4")

        if is_num_literal(op2) and ri:
            self.ins(ri, f"{d}, {op1_reg}", to_int(op2))
        elif is_num_literal(op2) and op == '-':
            self.ins("addi", f"{d}, {op1_reg}", -to_int(op2))
        elif is_num_literal(op2):
            self.ins("li", "t5", to_int(op2))
            self.ins(rr, f"{d}, {op1_reg}", "t5")
        else:
            if re.match(r'^(\w+)\[(\w+)\]$', op2):
                self._resolve_operand(op2, "t5")
                op2_reg = "t5"
            else:
                op2_reg = self.var_reg_or_load(op2, "t5")
            if op1_reg == op2_reg and op1_reg in ("t4", "t5"):
                self.load(op2, "t5")
                op2_reg = "t5"
            self.ins(rr, f"{d}, {op1_reg}", op2_reg)

        self.flush_dst(dst_var, d)

    def _resolve_char_literal(self, val):
        escape_map = {
            "'\\0'": "0",  r"'\0'": "0",
            "'\\n'": "10", r"'\n'": "10",
            "'\\t'": "9",  r"'\t'": "9",
            "'\\r'": "13", r"'\r'": "13",
            "'\\\\'": "92",
        }
        if val in escape_map:
            return escape_map[val]
        m = re.match(r"^'(.)'$", val)
        if m:
            return str(ord(m.group(1)))
        m = re.match(r"^'(\d+)'$", val)
        if m:
            return m.group(1)
        return val

    def _resolve_operand(self, raw, scratch):
        m = re.match(r'^(\w+)\[(\w+)\]$', raw)
        if m:
            arr, off = m.group(1), m.group(2)
            self.arr_addr(arr, off, scratch)
            if get_type(arr) in ('float', 'double'):
                self.ins("flw", "fa0", f"0({scratch})")
                self.ins("fcvt.d.s", "fa0", "fa0")
                return "FA0_ARRAY"
            ins = load_ins(arr) if arr in _var_types else 'lw'
            self.ins(ins, scratch, f"0({scratch})")
            return scratch
        return raw

    def emit_array_data(self):
        if not self._arrays:
            return
        self.e()
        self.e(".data")
        for aname in self._array_order:
            max_off = self._arrays[aname]
            n_words = (max_off // 4) + 1
            zeros   = ", ".join(["0"] * n_words)
            self.out.append(f"{aname}: .word {zeros}")

    def emit_array_la(self):
        for aname in self._array_order:
            reg = self.ra_alloc.reg(aname)
            if reg:
                self.ins("la", reg, aname)
            elif self.ra_alloc.is_spilled(aname):
                self.ins("la", "t4", aname)
                self.ins("sd",  "t4", f"{self.ra_alloc.spill_offset(aname)}(s0)")

    def arr_addr(self, arr, off, addr_reg="t4"):
        if is_int_literal(off):
            v = to_int(off)
            if self.ra_alloc.is_spilled(arr):
                self.ins("ld", addr_reg, f"{self.ra_alloc.spill_offset(arr)}(s0)")
                if v != 0:
                    self.ins("addi", addr_reg, addr_reg, v)
            else:
                base_r = self.ra_alloc.reg(arr)
                if base_r is None:
                    self.ins("la", addr_reg, arr)
                    if v != 0:
                        self.ins("addi", addr_reg, addr_reg, v)
                elif v == 0:
                    if base_r != addr_reg:
                        self.ins("mv", addr_reg, base_r)
                else:
                    self.ins("addi", addr_reg, base_r, v)
        else:
            off_r = self.var_reg_or_load(off, "t6")
            if off_r != "t6":
                self.ins("mv", "t6", off_r)
            if self.ra_alloc.is_spilled(arr):
                self.ins("ld", addr_reg, f"{self.ra_alloc.spill_offset(arr)}(s0)")
            else:
                base_r = self.ra_alloc.reg(arr)
                if base_r is None:
                    self.ins("la", addr_reg, arr)
                elif base_r != addr_reg:
                    self.ins("mv", addr_reg, base_r)
            self.ins("add", addr_reg, addr_reg, "t6")
        return addr_reg

    def emit_call(self, fname):
        param_types = getattr(self, '_fn_param_types', {}).get(fname, [])
        n = len(self.param_q)
        ordered = []
        for i in range(n):
            val = self.param_q[n - 1 - i]
            if i < len(param_types):
                ptype = param_types[i]
            else:
                ptype = 'int'
            val_str = str(val)
            if is_num_literal(val_str) and not is_int_literal(val_str):
                ptype = 'float'
            elif re.match(r'^[A-Za-z_]\w*$', val_str) and get_type(val_str) in ('float','double'):
                ptype = get_type(val_str)
            ordered.append((val, ptype))

        int_arg_idx   = 0
        float_arg_idx = 0
        stack_args = []
        for val, param_type in ordered:
            val_is_float = param_type in ('float', 'double')
            if val_is_float:
                if float_arg_idx < 8:
                    fa_reg = f"fa{float_arg_idx}"
                    float_arg_idx += 1
                    if is_num_literal(val) and not is_int_literal(val):
                        lname = f".flt{len(self.flt_lits)}"
                        self.flt_lits.append((lname, val))
                        self.ins("la",  "t4", lname)
                        self.ins("fld", fa_reg, "0(t4)")
                    elif is_int_literal(val):
                        self.ins("li", "t4", to_int(val))
                        self.ins("fcvt.d.w", fa_reg, "t4")
                    elif self.ra_alloc.is_spilled(val):
                        if get_type(val) in ('float', 'double'):
                            self.ins("fld", fa_reg, f"{self.ra_alloc.spill_offset(val)}(s0)")
                        else:
                            ins = load_ins(val)
                            self.ins(ins, "t4", f"{self.ra_alloc.spill_offset(val)}(s0)")
                            self.ins("fcvt.d.w", fa_reg, "t4")
                    else:
                        r = self.ra_alloc.reg(val)
                        if get_type(val) in ('float', 'double'):
                            self.ins("fmv.d.x", fa_reg, r if r else "zero")
                        else:
                            self.ins("fcvt.d.w", fa_reg, r if r else "zero")
                else:
                    stack_args.append((val, True))
            else:
                if int_arg_idx < 8:
                    a_reg = f"a{int_arg_idx}"
                    int_arg_idx += 1
                    self.load(val, a_reg)
                else:
                    stack_args.append((val, False))
        for val, is_flt in reversed(stack_args):
            if is_flt:
                if is_num_literal(val) and not is_int_literal(val):
                    lname = f".flt{len(self.flt_lits)}"
                    self.flt_lits.append((lname, val))
                    self.ins("la",  "t4", lname)
                    self.ins("fld", "fa0", "0(t4)")
                else:
                    self.load(val, "t4")
                    self.ins("fcvt.d.w", "fa0", "t4")
                self.ins("addi", "sp, sp", "-8")
                self.ins("fsd",  "fa0", "0(sp)")
            else:
                self.load(val, "t4")
                self.ins("addi", "sp, sp", "-8")
                self.ins("sd",   "t4", "0(sp)")
        self.ins("call", fname)
        if stack_args:
            self.ins("addi", "sp, sp", str(8 * len(stack_args)))
        self.param_q.clear()

    def _global_type_prescan(self):
        for _, instr in self.tac:
            m = re.match(r'^print(int|long|short|float|double|char)\s+(\w+)$', instr)
            if m:
                kind, var = m.group(1), m.group(2)
                tmap = {'int':'int','long':'long','short':'short',
                        'float':'float','double':'double','char':'char'}
                set_type(var, tmap[kind])
                continue

            m = re.match(r'^input(int|long|short|float|double|char)\s+(\w+)$', instr)
            if m:
                kind, var = m.group(1), m.group(2)
                tmap = {'int':'int','long':'long','short':'short',
                        'float':'float','double':'double','char':'char'}
                set_type(var, tmap[kind])
                continue

            m = re.match(r'^(\w+)\s*=\s*\((float|double|long|short|char|int)\)\s*\S+$', instr)
            if m:
                var, typ = m.group(1), m.group(2)
                set_type(var, typ)
                continue

        cur_fn_params = []
        in_fn = False
        fn_params = {}
        fn_param_types = {}

        for _, instr in self.tac:
            m_bf = re.match(r'^BeginFunc\s+(\w+)', instr)
            m_ef = re.match(r'^EndFunc\s+(\w+)', instr)
            m_pp = re.match(r'^PopParam\s+(\w+)$', instr)
            if m_bf:
                in_fn = True; cur_fn = m_bf.group(1); cur_fn_params = []
            elif m_pp and in_fn:
                cur_fn_params.append(m_pp.group(1))
            elif m_ef and in_fn:
                fn_params[cur_fn] = list(cur_fn_params)
                in_fn = False

        param_q_tmp = []
        calling_fn = None
        for _, instr in self.tac:
            m_pp = re.match(r'^PushParam\s+(\S+)$', instr)
            m_cx = re.match(r'^(?:\w+\s*=\s*)?Call\s+(\w+)$', instr)
            if m_pp:
                param_q_tmp.append(m_pp.group(1))
            elif m_cx:
                fname = m_cx.group(1)
                params = fn_params.get(fname, [])
                n = len(param_q_tmp)
                for i, pvar in enumerate(params):
                    pushed_val = param_q_tmp[n - 1 - i] if (n - 1 - i) >= 0 else None
                    if pushed_val and get_type(pvar) == 'int':
                        if is_num_literal(pushed_val) and not is_int_literal(pushed_val):
                            set_type(pvar, 'float')
                        elif re.match(r'^[A-Za-z_]\w*$', pushed_val) and \
                                get_type(pushed_val) in ('float','double'):
                            set_type(pvar, 'float')
                param_q_tmp.clear()

        changed = True
        while changed:
            changed = False
            for _, instr in self.tac:
                m = re.match(r'^(\w+)\s*=\s*([A-Za-z_]\w*)$', instr)
                if m:
                    dst, src = m.group(1), m.group(2)
                    if get_type(src) in ('float','double') and get_type(dst) not in ('float','double'):
                        set_type(dst, get_type(src)); changed = True
                    continue
                m = re.match(r'^(\w+)\s*=\s*(\S+)\s*[+\-*/]\s*(\S+)$', instr)
                if m:
                    dst, op1, op2 = m.group(1), m.group(2), m.group(3)
                    op1_float = (is_num_literal(op1) and not is_int_literal(op1)) or \
                                get_type(op1) in ('float','double')
                    op2_float = (is_num_literal(op2) and not is_int_literal(op2)) or \
                                get_type(op2) in ('float','double')
                    if (op1_float or op2_float) and get_type(dst) not in ('float','double'):
                        set_type(dst, 'float'); changed = True

        for fname, params in fn_params.items():
            fn_param_types[fname] = [get_type(p) for p in params]

        fn_return_types = {}
        cur_fn = None
        for _, instr in self.tac:
            m_bf = re.match(r'^BeginFunc\s+(\w+)', instr)
            m_ef = re.match(r'^EndFunc\s+(\w+)', instr)
            m_ret = re.match(r'^Return\s+(\S+)$', instr)
            if m_bf:
                cur_fn = m_bf.group(1)
            elif m_ef:
                cur_fn = None
            elif m_ret and cur_fn:
                val = m_ret.group(1)
                if (is_num_literal(val) and not is_int_literal(val)) or \
                        get_type(val) in ('float', 'double'):
                    fn_return_types[cur_fn] = 'float'
                elif cur_fn not in fn_return_types:
                    fn_return_types[cur_fn] = 'int'

        self._fn_param_types = fn_param_types
        self._fn_return_types = fn_return_types

    def convert(self, tac_text):
        for raw in tac_text.splitlines():
            if not raw.strip():
                continue
            lnum, instr = parse_line(raw)
            if lnum is not None:
                self.tac.append((lnum, instr))

        self.tac = _copy_propagate(self.tac)
        self.collect_targets()
        self._global_type_prescan()

        self.e()
        self.e(".text")

        func_order = [instr.split()[1]
                      for _, instr in self.tac
                      if re.match(r'^BeginFunc\s+', instr)]

        fn_slices = {}
        cur_fn, cur_slice = None, []
        for _, instr in self.tac:
            m_bf = re.match(r'^BeginFunc\s+(\w+)', instr)
            is_ef = bool(re.match(r'^EndFunc\b', instr))
            if m_bf:
                cur_fn    = m_bf.group(1)
                cur_slice = [instr]
            elif is_ef and cur_fn:
                cur_slice.append(instr)
                fn_slices[cur_fn] = cur_slice
                cur_fn = None
            elif cur_fn:
                cur_slice.append(instr)

        is_bare_tac        = not func_order
        has_main_func      = 'main' in func_order

        global_tac = []
        _in_fn = False
        for lnum, instr in self.tac:
            if re.match(r'^BeginFunc\b', instr):
                _in_fn = True
            elif re.match(r'^EndFunc\b', instr):
                _in_fn = False
            elif not _in_fn:
                global_tac.append((lnum, instr))

        is_mixed_tac       = bool(func_order) and bool(global_tac) and not has_main_func
        is_funcs_only      = bool(func_order) and not has_main_func and not global_tac

        if is_bare_tac:
            all_instrs = [instr for _, instr in self.tac]
            self.ra_alloc._array_names = set(self._arrays.keys())
            self._preseed_float_types(all_instrs)
            self.ra_alloc.set_safe_scratch_temps(list(self.tac))
            self.ra_alloc.run_graph_coloring(list(self.tac))
            bare_has_calls = self._fn_has_calls(all_instrs)
            self._prescan_function(all_instrs)
            sregs    = list(self.ra_alloc.used_sregs)
            fp_sregs = list(self.ra_alloc.used_fp_sregs)

            bare_needs_ra = bare_has_calls
            bare_needs_fp = self._fn_needs_fp(self.ra_alloc)

            if not bare_needs_fp:
                all_tac_temps_bare = set()
                for line in all_instrs:
                    for tok in re.findall(r'\bt\d+\b', line):
                        if get_type(tok) not in ('float', 'double'):
                            all_tac_temps_bare.add(tok)
                if all_tac_temps_bare - self.ra_alloc._safe_scratch:
                    bare_needs_fp = True

            float_reserve = 0
            if bare_needs_fp:
                fp_pool = set(RegAlloc.FP_CALLER_POOL) | set(RegAlloc.FP_SAVED_POOL)
                for line in all_instrs:
                    for tok in re.findall(r'\b[A-Za-z_]\w*\b', line):
                        if (get_type(tok) in ('float', 'double')
                                and tok not in self.ra_alloc._map
                                and tok not in self.ra_alloc._spill):
                            float_reserve += 1
                float_reserve = min(float_reserve, 8)

            base_slots = self.ra_alloc.spill_slots
            bare_frame = _compute_frame(len(sregs) + len(fp_sregs),
                                        base_slots + float_reserve,
                                        bare_needs_ra, bare_needs_fp)
            self._bare_sregs    = sregs
            self._bare_fp_sregs = fp_sregs
            self._bare_frame    = bare_frame
            self._bare_needs_ra = bare_needs_ra
            self._bare_needs_fp = bare_needs_fp

            self.e()
            self.e("    .globl   main")
            self.lbl("main")
            self.e("")

            if bare_frame > 0:
                self.ins("addi", "sp, sp", f"-{bare_frame}")
            off = bare_frame
            if bare_needs_ra:
                off -= 8; self.ins("sd", "ra", f"{off}(sp)")
            if bare_needs_fp:
                off -= 8; self.ins("sd", "s0", f"{off}(sp)")
            for reg in sregs:
                off -= 8; self.ins("sd", reg, f"{off}(sp)")
            for reg in fp_sregs:
                off -= 8; self.ins("fsd", reg, f"{off}(sp)")
            if bare_needs_fp:
                self.ins("addi", "s0, sp", str(bare_frame))

            self.in_main = True
            self.emit_array_la()

        if is_mixed_tac:
            global_instrs = [instr for _, instr in global_tac]
            fn_slices['__global_main__'] = (
                ['BeginFunc main 0'] + global_instrs + ['EndFunc main']
            )

        if is_mixed_tac:
            global_lnums = {lnum for lnum, _ in global_tac}
            for lnum, instr in self.tac:
                if lnum not in global_lnums:
                    self._dispatch(lnum, instr, fn_slices)

            self.prologue('main', 0, fn_slices['__global_main__'])
            self.in_main = True
            self.emit_array_la()
            for lnum, instr in global_tac:
                self._dispatch(lnum, instr, fn_slices)
            self.e()
            self.ins("li",   "a0", 0)
            self.ins("j",    self.epilogue_lbl)
            self.epilogue()
        elif is_funcs_only:
            for lnum, instr in self.tac:
                self._dispatch(lnum, instr, fn_slices)
            self.e()
            self.e("    # -- synthetic main (functions-only TAC) --")
            self.ins(".globl", "main")
            self.lbl("main")
            self.ins("addi", "sp, sp", "-16")
            self.ins("sd",   "ra",  "8(sp)")
            self.ins("li",   "a0", 0)
            self.ins("ld",   "ra",  "8(sp)")
            self.ins("addi", "sp, sp", "16")
            self.ins("ret")
        else:
            for lnum, instr in self.tac:
                self._dispatch(lnum, instr, fn_slices)

        def _emit_bare_exit():
            bf       = getattr(self, '_bare_frame',    16)
            bra      = getattr(self, '_bare_needs_ra', False)
            bfp      = getattr(self, '_bare_needs_fp', False)
            bsregs   = getattr(self, '_bare_sregs',   [])
            bfpsregs = getattr(self, '_bare_fp_sregs', [])
            off = bf
            if bra:
                off -= 8; ra_off = off
            if bfp:
                off -= 8; s0_off = off
            sreg_offs = []
            for _ in bsregs:
                off -= 8; sreg_offs.append(off)
            fp_sreg_offs = []
            for _ in bfpsregs:
                off -= 8; fp_sreg_offs.append(off)
            if bra:
                self.ins("ld", "ra", f"{ra_off}(sp)")
            for reg, roff in zip(bsregs, sreg_offs):
                self.ins("ld", reg, f"{roff}(sp)")
            for reg, roff in zip(bfpsregs, fp_sreg_offs):
                self.ins("fld", reg, f"{roff}(sp)")
            if bfp:
                self.ins("ld", "s0", f"{s0_off}(sp)")
            if bf > 0:
                self.ins("addi", "sp, sp", str(bf))
            self.ins("li",  "a0", 0)
            self.ins("ret")

        global_missing = self._global_targets - self._global_lines
        if global_missing and not is_mixed_tac:
            self.e()
            self.e("    # -- program exit / out-of-range jump targets --")
            for n in sorted(global_missing):
                self.lbl(self.tac_lbl(n))
                if is_bare_tac:
                    _emit_bare_exit()
                else:
                    self.ins("nop")
        elif is_bare_tac:
            self.e()
            _emit_bare_exit()

        self.e()
        self.e(".data")

        FMT_TABLE = {
            '.fmt_int':        '.fmt_int:        .asciz "%d\\n"',
            '.fmt_long':       '.fmt_long:       .asciz "%ld\\n"',
            '.fmt_char':       '.fmt_char:       .asciz "%c\\n"',
            '.fmt_float':      '.fmt_float:      .asciz "%f\\n"',
            '.fmt_str':        '.fmt_str:        .asciz "%s\\n"',
            '.fmt_scan_int':   '.fmt_scan_int:   .asciz "%d"',
            '.fmt_scan_long':  '.fmt_scan_long:  .asciz "%ld"',
            '.fmt_scan_float': '.fmt_scan_float: .asciz "%lf"',
        }
        for key in ['.fmt_int','.fmt_long','.fmt_char','.fmt_float','.fmt_str',
                    '.fmt_scan_int','.fmt_scan_long','.fmt_scan_float']:
            if key in self._used_fmts:
                self.out.append(FMT_TABLE[key])
        for aname in self._array_order:
            max_off = self._arrays[aname]
            stride  = getattr(self, '_array_stride', {}).get(aname, 4)
            atype   = get_type(aname)
            n_elems = (max_off // stride) + 1 if stride > 0 else (max_off // 4) + 1
            if atype == 'char':
                zeros = ", ".join(["0"] * n_elems)
                self.out.append(f"{aname}: .byte {zeros}")
            elif atype == 'short':
                zeros = ", ".join(["0"] * n_elems)
                self.out.append(f"{aname}: .short {zeros}")
            elif atype in ('float', 'double'):
                zeros = ", ".join(["0"] * n_elems)
                self.out.append(f"{aname}: .word {zeros}")
            else:
                if atype == 'long':
                    zeros = ", ".join(["0"] * n_elems)
                    self.out.append(f"{aname}: .dword {zeros}")
                else:
                    zeros = ", ".join(["0"] * n_elems)
                    self.out.append(f"{aname}: .word {zeros}")
        for lname, s in self.str_lits:
            self.out.append(f"    {lname}: .asciz {s}")
        for lname, v in self.flt_lits:
            self.out.append(f"    {lname}: .double {v}")

        if self._used_externs:
            text_idx = next((i for i, l in enumerate(self.out) if l.strip() == '.text'), None)
            if text_idx is not None:
                extern_lines = [f"    .extern  {sym}" for sym in sorted(self._used_externs)]
                self.out[text_idx:text_idx] = extern_lines

        return "\n".join(self.out) + "\n"

    def _dispatch(self, lnum, instr, fn_slices=None):

        if lnum in self.jump_targets and not re.match(r'^BeginFunc\b', instr):
            self.e()
            self.lbl(self.tac_lbl(lnum))

        if instr.startswith("//"):
            #self.e(f"    # [ELIM] {instr}")
            return

        m = re.match(r'^BeginFunc\s+(\w+)(?:\s+(\d+))?$', instr)
        if m:
            fname  = m.group(1)
            nparams = int(m.group(2) or 0)
            slice_ = (fn_slices or {}).get(fname, [])
            self.prologue(fname, nparams, slice_)
            return

        m = re.match(r'^EndFunc\b', instr)
        if m:
            self.epilogue()
            return

        m = re.match(r'^PopParam\s+(\w+)$', instr)
        if m:
            var = m.group(1)
            vtype = get_type(var)
            if vtype in ('float', 'double'):
                if self.float_pop_idx < 8:
                    arg_reg = f"fa{self.float_pop_idx}"
                    self.float_pop_idx += 1
                    self.e(f"    # PopParam {var} (float) <- {arg_reg}")
                    off = self.ra_alloc.force_spill(var)
                    if not hasattr(self.ra_alloc, '_float_vars'):
                        self.ra_alloc._float_vars = set()
                    self.ra_alloc._float_vars.add(var)
                    self.ins("fsd", arg_reg, f"{off}(s0)")
                else:
                    stack_idx = self.float_pop_idx - 8
                    self.float_pop_idx += 1
                    caller_off = stack_idx * 8
                    self.e(f"    # PopParam {var} (float) <- stack[{stack_idx}] @ {caller_off}(s0)")
                    off = self.ra_alloc.force_spill(var)
                    if not hasattr(self.ra_alloc, '_float_vars'):
                        self.ra_alloc._float_vars = set()
                    self.ra_alloc._float_vars.add(var)
                    self.ins("fld", "ft4", f"{caller_off}(s0)")
                    self.ins("fsd", "ft4", f"{off}(s0)")
            else:
                if self.int_pop_idx < 8:
                    arg_reg = f"a{self.int_pop_idx}"
                    self.int_pop_idx += 1
                    self.e(f"    # PopParam {var} <- {arg_reg}")
                    self.store_var(var, arg_reg)
                else:
                    stack_idx = self.int_pop_idx - 8
                    self.int_pop_idx += 1
                    caller_off = stack_idx * 8
                    self.e(f"    # PopParam {var} <- stack[{stack_idx}] @ {caller_off}(s0)")
                    self.ins("ld", "t4", f"{caller_off}(s0)")
                    self.store_var(var, "t4")
            return

        m = re.match(r'^PushParam\s+(.+)$', instr)
        if m:
            self.param_q.append(m.group(1).strip())
            return

        m = re.match(r'^(\w+)\s*=\s*Call\s+(\w+)$', instr)
        if m:
            dst_var, fname = m.group(1), m.group(2)
            self.emit_call(fname)
            ret_is_float = get_type(dst_var) in ('float', 'double') or \
                           (hasattr(self, '_fn_return_types') and
                            self._fn_return_types.get(fname) in ('float', 'double'))
            if ret_is_float:
                off = self.ra_alloc.force_spill(dst_var)
                self.ins("fsd", "fa0", f"{off}(s0)")
                if not hasattr(self.ra_alloc, '_float_vars'):
                    self.ra_alloc._float_vars = set()
                self.ra_alloc._float_vars.add(dst_var)
                set_type(dst_var, 'float')
            else:
                self.store_var(dst_var, "a0")
            return

        m = re.match(r'^Call\s+(\w+)$', instr)
        if m:
            self.emit_call(m.group(1))
            return

        m = re.match(r'^Return\s+(.+)$', instr)
        if m:
            val = m.group(1).strip()
            val_is_float = (
                (is_num_literal(val) and not is_int_literal(val)) or
                get_type(val) in ('float', 'double') or
                (hasattr(self.ra_alloc, '_float_vars') and val in self.ra_alloc._float_vars)
            )
            if val_is_float:
                if self.ra_alloc.is_spilled(val):
                    self.ins("fld", "fa0", f"{self.ra_alloc.spill_offset(val)}(s0)")
                elif is_num_literal(val) and not is_int_literal(val):
                    lname = f".flt{len(self.flt_lits)}"
                    self.flt_lits.append((lname, val))
                    self.ins("la",  "t4", lname)
                    self.ins("fld", "fa0", "0(t4)")
                else:
                    r = self.ra_alloc.reg(val)
                    self.ins("fcvt.d.w", "fa0", r if r else "zero")
            else:
                src_r = self.operand(val, "a0")
                if src_r != "a0":
                    self.ins("mv", "a0", src_r)
            self.ins("j", self.epilogue_lbl)
            return

        if instr.strip() == "Return":
            self.ins("li", "a0", 0)
            self.ins("j", self.epilogue_lbl)
            return

        m = re.match(r'^goto\s+(\d+)$', instr)
        if m:
            tgt = int(m.group(1))
            next_lnum = None
            for ll, _ in self.tac:
                if ll > lnum:
                    next_lnum = ll
                    break
            if tgt != next_lnum:
                self.ins("j", self.tac_lbl(tgt))
            return

        m = re.match(
            r'^if\s+(\S+)\s+(==|!=|<=|>=|<<|>>|<|>|lt|gt|le|ge|eq|ne)\s+(\S+)\s+goto\s+(\d+)$',
            instr)
        if m:
            lhs, op, rhs, tgt = m.group(1), m.group(2), m.group(3), int(m.group(4))
            self.emit_branch(lhs, op, rhs, self.tac_lbl(tgt))
            return

        m = re.match(r'^print(int|long|short|float|double|char|string)\s+(.+)$', instr)
        if m:
            kind, val = m.group(1), m.group(2).strip()
            if re.match(r'^[A-Za-z_]\w*$', val):
                if kind == 'long':   set_type(val, 'long')
                elif kind == 'short': set_type(val, 'short')
                elif kind == 'double': set_type(val, 'double')
                elif kind == 'float':  set_type(val, 'float')
                elif kind == 'char':   set_type(val, 'char')

            if kind in ("int", "short"):
                self.use_extern("printf"); self.use_fmt(".fmt_int")
                self.ins("la", "a0", ".fmt_int")
                arr_m = re.match(r'^(\w+)\[(\w+)\]$', val)
                if arr_m:
                    self._resolve_operand(val, "t4")
                    self.ins("mv", "a1", "t4")
                else:
                    r = self.operand(val, "t4")
                    if r != "a1":
                        self.ins("mv", "a1", r)
                self.ins("call", "printf")
            elif kind == "long":
                self.use_extern("printf"); self.use_fmt(".fmt_long")
                self.ins("la", "a0", ".fmt_long")
                arr_m = re.match(r'^(\w+)\[(\w+)\]$', val)
                if arr_m:
                    self._resolve_operand(val, "t4")
                    self.ins("mv", "a1", "t4")
                else:
                    r = self.operand(val, "t4")
                    if r != "a1":
                        self.ins("mv", "a1", r)
                self.ins("call", "printf")
            elif kind == "char":
                val = self._resolve_char_literal(val)
                is_str_var = (hasattr(self.ra_alloc, '_string_vars') and
                              val in self.ra_alloc._string_vars)
                arr_m = re.match(r'^(\w+)\[(\w+)\]$', val)
                if is_str_var:
                    self.use_extern("printf"); self.use_fmt(".fmt_str")
                    self.ins("la", "a0", ".fmt_str")
                    r = self.operand(val, "t4")
                    if r != "a1":
                        self.ins("mv", "a1", r)
                    self.ins("call", "printf")
                elif arr_m:
                    self.use_extern("printf"); self.use_fmt(".fmt_char")
                    self.ins("la", "a0", ".fmt_char")
                    self._resolve_operand(val, "t4")
                    self.ins("mv", "a1", "t4")
                    self.ins("call", "printf")
                else:
                    self.use_extern("printf"); self.use_fmt(".fmt_char")
                    self.ins("la", "a0", ".fmt_char")
                    r = self.operand(val, "t4")
                    if r != "a1":
                        self.ins("mv", "a1", r)
                    self.ins("call", "printf")
            elif kind in ("float", "double"):
                self.use_extern("printf"); self.use_fmt(".fmt_float")
                self.ins("la", "a0", ".fmt_float")
                arr_m = re.match(r'^(\w+)\[(\w+)\]$', val)
                if arr_m and get_type(arr_m.group(1)) in ('float', 'double'):
                    self._resolve_operand(val, "t4")
                elif is_num_literal(val):
                    lname = f".flt{len(self.flt_lits)}"
                    self.flt_lits.append((lname, val))
                    self.ins("la",  "t4",  lname)
                    self.ins("fld", "fa0", "0(t4)")
                else:
                    self._load_float_var_into(val, "fa0")
                self.ins("fmv.x.d", "a1", "fa0")
                self.ins("call",    "printf")
            elif kind == "string":
                self.use_extern("printf"); self.use_fmt(".fmt_str")
                self.ins("la", "a0", ".fmt_str")
                if val.startswith('"'):
                    slbl = f".str{len(self.str_lits)}"
                    self.str_lits.append((slbl, val))
                    self.ins("la", "a1", slbl)
                else:
                    r = self.operand(val, "t4")
                    if r != "a1":
                        self.ins("mv", "a1", r)
                self.ins("call", "printf")
            return

        m = re.match(r'^input(int|long|short|float|double|char|string)\s+(\w+)$', instr)
        if m:
            kind, var = m.group(1), m.group(2)
            if kind == 'long':   set_type(var, 'long')
            elif kind == 'short': set_type(var, 'short')
            elif kind == 'double': set_type(var, 'double')
            elif kind == 'float':  set_type(var, 'float')
            elif kind == 'char':   set_type(var, 'char')

            if kind in ("int", "short"):
                self.use_extern("scanf"); self.use_fmt(".fmt_scan_int")
                self.ins("addi", "sp", "sp", "-8")
                self.ins("la",   "a0", ".fmt_scan_int")
                self.ins("mv",   "a1", "sp")
                self.ins("call", "scanf")
                self.ins("lw",   "t4", "0(sp)")
                self.ins("addi", "sp", "sp", "8")
                self.store_var(var, "t4")
            elif kind == "long":
                self.use_extern("scanf"); self.use_fmt(".fmt_scan_long")
                self.ins("addi", "sp", "sp", "-8")
                self.ins("la",   "a0", ".fmt_scan_long")
                self.ins("mv",   "a1", "sp")
                self.ins("call", "scanf")
                self.ins("ld",   "t4", "0(sp)")
                self.ins("addi", "sp", "sp", "8")
                self.store_var(var, "t4")
            elif kind == "char":
                self.use_extern("getchar")
                self.ins("call", "getchar")
                self.store_var(var, "a0")
            elif kind in ("float", "double"):
                self.use_extern("scanf"); self.use_fmt(".fmt_scan_float")
                self.ins("addi", "sp", "sp", "-8")
                self.ins("la",   "a0", ".fmt_scan_float")
                self.ins("mv",   "a1", "sp")
                self.ins("call", "scanf")
                self.ins("fld",  "fa0", "0(sp)")
                self.ins("addi", "sp", "sp", "8")
                off = self.ra_alloc.force_spill(var)
                self.ins("fsd",  "fa0", f"{off}(s0)")
                if not hasattr(self.ra_alloc, '_float_vars'):
                    self.ra_alloc._float_vars = set()
                self.ra_alloc._float_vars.add(var)
            elif kind == "string":
                self.use_extern("fgets")
                self.load(var, "a0")
                self.ins("li",   "a1", "256")
                self.ins("la",   "a2", "stdin")
                self.ins("lw",   "a2", "0(a2)")
                self.ins("call", "fgets")
            return

        m = re.match(r'^(\w+)\[(\d+)\]\s*=\s*(.+)$', instr)
        if m:
            arr, byte_off, val = m.group(1), int(m.group(2)), m.group(3).strip()
            val = self._resolve_char_literal(val)
            self.arr_addr(arr, str(byte_off), "t4")
            arr_is_float = get_type(arr) in ('float', 'double')
            rhs_m = re.match(r'^(\w+)\[(\w+)\]$', val)
            if arr_is_float:
                if rhs_m:
                    self.arr_addr(rhs_m.group(1), rhs_m.group(2), "t5")
                    self.ins("flw", "fa1", "0(t5)")
                    self.ins("fcvt.d.s", "fa1", "fa1")
                elif is_num_literal(val) and not is_int_literal(val):
                    lname = f".flt{len(self.flt_lits)}"
                    self.flt_lits.append((lname, val))
                    self.ins("la",  "t5", lname)
                    self.ins("fld", "fa1", "0(t5)")
                elif is_int_literal(val):
                    self.ins("li", "t5", to_int(val))
                    self.ins("fcvt.s.w", "fa1", "t5")
                else:
                    if self.ra_alloc.is_spilled(val):
                        self.ins("fld", "fa1", f"{self.ra_alloc.spill_offset(val)}(s0)")
                    else:
                        r = self.ra_alloc.reg(val)
                        self.ins("fcvt.s.w", "fa1", r if r else "zero")
                self.ins("fcvt.s.d", "fa1", "fa1")
                self.ins("fsw", "fa1", "0(t4)")
            else:
                if rhs_m:
                    self.arr_addr(rhs_m.group(1), rhs_m.group(2), "t5")
                    ins = load_ins(rhs_m.group(1)) if rhs_m.group(1) in _var_types else 'lw'
                    self.ins(ins, "t3", "0(t5)")
                else:
                    self.load(val, "t3")
                s_ins = store_ins(arr) if arr in _var_types else 'sw'
                self.ins(s_ins, "t3", "0(t4)")
            return

        m = re.match(r'^(\w+)\[(\w+)\]\s*=\s*(.+)$', instr)
        if m:
            arr, off_var, val = m.group(1), m.group(2), m.group(3).strip()
            val = self._resolve_char_literal(val)
            arr_is_float = get_type(arr) in ('float', 'double')
            rhs_m = re.match(r'^(\w+)\[(\w+)\]$', val)
            if arr_is_float:
                if rhs_m:
                    self.arr_addr(rhs_m.group(1), rhs_m.group(2), "t5")
                    self.ins("flw", "fa1", "0(t5)")
                elif is_num_literal(val) and not is_int_literal(val):
                    lname = f".flt{len(self.flt_lits)}"
                    self.flt_lits.append((lname, val))
                    self.ins("la",  "t5", lname)
                    self.ins("fld", "fa1", "0(t5)")
                elif is_int_literal(val):
                    self.ins("li", "t5", to_int(val))
                    self.ins("fcvt.s.w", "fa1", "t5")
                else:
                    if self.ra_alloc.is_spilled(val):
                        self.ins("fld", "fa1", f"{self.ra_alloc.spill_offset(val)}(s0)")
                    else:
                        r = self.ra_alloc.reg(val)
                        self.ins("fcvt.s.w", "fa1", r if r else "zero")
                self.ins("fcvt.s.d", "fa1", "fa1")
                self.arr_addr(arr, off_var, "t4")
                self.ins("fsw", "fa1", "0(t4)")
            else:

                off_r = self.var_reg_or_load(off_var, "t6")
                if off_r != "t6":
                    self.ins("mv", "t6", off_r)

                if rhs_m:
                    lhs_base_r = self.ra_alloc.reg(arr)
                    if self.ra_alloc.is_spilled(arr):
                        self.ins("ld", "t4", f"{self.ra_alloc.spill_offset(arr)}(s0)")
                    elif lhs_base_r and lhs_base_r != "t4":
                        self.ins("mv", "t4", lhs_base_r)
                    elif lhs_base_r is None:
                        self.ins("la", "t4", arr)
                    self.ins("add", "t4", "t4", "t6")
                    rhs_arr, rhs_off = rhs_m.group(1), rhs_m.group(2)
                    self.arr_addr(rhs_arr, rhs_off, "t5")
                    rhs_ins = load_ins(rhs_arr) if rhs_arr in _var_types else 'lw'
                    self.ins(rhs_ins, "t3", "0(t5)")
                    s_ins = store_ins(arr) if arr in _var_types else 'sw'
                    self.ins(s_ins, "t3", "0(t4)")
                else:
                    self.load(val, "t3")
                    lhs_base_r = self.ra_alloc.reg(arr)
                    if self.ra_alloc.is_spilled(arr):
                        self.ins("ld", "t4", f"{self.ra_alloc.spill_offset(arr)}(s0)")
                    elif lhs_base_r and lhs_base_r != "t4":
                        self.ins("mv", "t4", lhs_base_r)
                    elif lhs_base_r is None:
                        self.ins("la", "t4", arr)
                    self.ins("add", "t4", "t4", "t6")
                    s_ins = store_ins(arr) if arr in _var_types else 'sw'
                    self.ins(s_ins, "t3", "0(t4)")
            return

        m = re.match(r'^(\w+)\s*=\s*-\s+(\w+)\[(\w+)\]$', instr)
        if m:
            dst_var, arr, off = m.group(1), m.group(2), m.group(3)
            self.e(f"    # unary minus of array element {arr}[{off}]")
            self.arr_addr(arr, off, "t4")
            self.ins("lw", "t4", "0(t4)")
            d = self.dst_reg(dst_var)
            self.ins("neg", d, "t4")
            self.flush_dst(dst_var, d)
            return

        m = re.match(r'^(\w+)\s*=\s*-\s*(\w+)(\[.+\])$', instr)
        if m:
            dst_var, arr, subscript = m.group(1), m.group(2), m.group(3)
            self.e(f"    # unary minus of {arr}{subscript}")
            idx_m = re.match(r'\[(\w+)\]', subscript)
            if idx_m:
                self.arr_addr(arr, idx_m.group(1), "t4")
                self.ins("lw", "t4", "0(t4)")
            else:
                self.load(arr, "t4")
            d = self.dst_reg(dst_var)
            self.ins("neg", d, "t4")
            self.flush_dst(dst_var, d)
            return

        m = re.match(r'^(\w+)\s*=\s*(\w+)\[(\w+)\]$', instr)
        if m:
            dst_var, arr, off = m.group(1), m.group(2), m.group(3)
            self.e(f"    # array read {dst_var} = {arr}[{off}]")
            arr_type = get_type(arr)
            if arr_type in ('float', 'double'):
                if is_int_literal(off):
                    base_r = self.ra_alloc.reg(arr) if not self.ra_alloc.is_spilled(arr) else None
                    if self.ra_alloc.is_spilled(arr):
                        self.ins("ld", "t4", f"{self.ra_alloc.spill_offset(arr)}(s0)")
                        self.ins("flw", "fa0", f"{to_int(off)}(t4)")
                    else:
                        self.ins("flw", "fa0", f"{to_int(off)}({base_r})")
                else:
                    self.arr_addr(arr, off, "t4")
                    self.ins("flw", "fa0", "0(t4)")
                self.ins("fcvt.d.s", "fa0", "fa0")
                off_dst = self.ra_alloc.force_spill(dst_var)
                self.ins("fsd", "fa0", f"{off_dst}(s0)")
                if not hasattr(self.ra_alloc, '_float_vars'):
                    self.ra_alloc._float_vars = set()
                self.ra_alloc._float_vars.add(dst_var)
                set_type(dst_var, 'float')
            else:
                d = self.dst_reg(dst_var)
                ld_ins = load_ins(arr) if arr in _var_types else 'lw'
                if is_int_literal(off):
                    base_r = self.ra_alloc.reg(arr) if not self.ra_alloc.is_spilled(arr) else None
                    if self.ra_alloc.is_spilled(arr):
                        self.ins("ld", "t4", f"{self.ra_alloc.spill_offset(arr)}(s0)")
                        self.ins(ld_ins, d, f"{to_int(off)}(t4)")
                    else:
                        self.ins(ld_ins, d, f"{to_int(off)}({base_r})")
                else:
                    self.arr_addr(arr, off, "t4")
                    self.ins(ld_ins, d, "0(t4)")
                self.flush_dst(dst_var, d)
            return

        m = re.match(r'^(\w+)\s*=\s*\((\w+)\)\s*(".*?"|\S+)$', instr)
        if m:
            dst_var, cast_type, src = m.group(1), m.group(2), m.group(3)
            self.e(f"    # cast ({cast_type})")

            _type_map = {
                'long': 'long', 'short': 'short', 'char': 'char',
                'float': 'float', 'double': 'double', 'int': 'int'
            }
            if cast_type in _type_map:
                set_type(dst_var, _type_map[cast_type])

            int_types   = {"int", "long", "short", "char", "bool",
                           "unsigned", "uint", "int32", "int64", "int16",
                           "int8", "byte", "word"}
            float_types = {"float", "double", "float32", "float64", "real"}

            if cast_type in float_types:
                off = self.ra_alloc.force_spill(dst_var)

                if is_num_literal(src) and not is_int_literal(src):
                    lname = f".flt{len(self.flt_lits)}"
                    self.flt_lits.append((lname, src))
                    self.ins("la",  "t4",  lname)
                    self.ins("fld", "fa0", "0(t4)")
                elif is_int_literal(src):
                    self.ins("li",         "t4",  str(to_int(src)))
                    self.ins("fcvt.d.w",   "fa0", "t4")
                elif self.ra_alloc.is_spilled(src):
                    src_already_float = (
                        get_type(src) in ('float', 'double') or
                        (hasattr(self.ra_alloc, '_float_vars') and src in self.ra_alloc._float_vars)
                    )
                    if src_already_float:
                        self.ins("fld", "fa0", f"{self.ra_alloc.spill_offset(src)}(s0)")
                    else:
                        self.ins("lw",         "t4",  f"{self.ra_alloc.spill_offset(src)}(s0)")
                        self.ins("fcvt.d.w",   "fa0", "t4")
                else:
                    r = self.ra_alloc.reg(src)
                    if r:
                        src_already_float = (
                            get_type(src) in ('float', 'double') or
                            (hasattr(self.ra_alloc, '_float_vars') and src in self.ra_alloc._float_vars)
                        )
                        if src_already_float:
                            self.ins("fmv.d.x", "fa0", r)
                        else:
                            self.ins("fcvt.d.w", "fa0", r)
                    else:
                        self.ins("fcvt.d.w", "fa0", "zero")

                self.ins("fsd", "fa0", f"{off}(s0)")
                if not hasattr(self.ra_alloc, '_float_vars'):
                    self.ra_alloc._float_vars = set()
                self.ra_alloc._float_vars.add(dst_var)

            elif cast_type in int_types or cast_type.startswith("("):
                if src.startswith('"'):
                    slbl = f".str{len(self.str_lits)}"
                    self.str_lits.append((slbl, src))
                    d = self.dst_reg(dst_var)
                    self.ins("la", d, slbl)
                    self.flush_dst(dst_var, d)
                    if not hasattr(self.ra_alloc, '_string_vars'):
                        self.ra_alloc._string_vars = set()
                    self.ra_alloc._string_vars.add(dst_var)
                    return

                src = self._resolve_char_literal(src)

                d = self.dst_reg(dst_var)

                src_is_float_lit = is_num_literal(src) and not is_int_literal(src)
                src_is_float_var = (
                    get_type(src) in ('float', 'double') or
                    (hasattr(self.ra_alloc, '_float_vars') and src in self.ra_alloc._float_vars)
                )

                if src_is_float_lit:
                    lname = f".flt{len(self.flt_lits)}"
                    self.flt_lits.append((lname, src))
                    self.ins("la",  "t4",  lname)
                    self.ins("fld", "fa0", "0(t4)")
                    self.ins("fcvt.w.d", d, "fa0", "rtz")
                elif src_is_float_var:
                    if self.ra_alloc.is_spilled(src):
                        self.ins("fld", "fa0", f"{self.ra_alloc.spill_offset(src)}(s0)")
                    else:
                        r = self.ra_alloc.reg(src)
                        self.ins("fmv.d.x", "fa0", r if r else "zero")
                    self.ins("fcvt.w.d", d, "fa0", "rtz")
                else:
                    src_r = self.operand(src, "t4")
                    if cast_type == "char":
                        if src_r != d:
                            self.ins("mv", d, src_r)
                        self.ins("slli", d, d, "56")
                        self.ins("srai", d, d, "56")
                    elif cast_type == "short":
                        if src_r != d:
                            self.ins("mv", d, src_r)
                        self.ins("slli", d, d, "48")
                        self.ins("srai", d, d, "48")
                    elif cast_type == "bool":
                        self.ins("snez", d, src_r)
                    else:
                        if src_r != d:
                            self.ins("mv", d, src_r)

                self.flush_dst(dst_var, d)

            else:
                d = self.dst_reg(dst_var)
                src_r = self.operand(src, "t4")
                if src_r != d:
                    self.ins("mv", d, src_r)
                self.flush_dst(dst_var, d)
            return

        m = re.match(r'^(\w+)\s*=\s*-\s+(\S+)$', instr)
        if m:
            dst_var, src = m.group(1), m.group(2)
            src = self._resolve_char_literal(src)
            d = self.dst_reg(dst_var)
            if is_int_literal(src):
                self.ins("li", d, str(-to_int(src)))
            else:
                src_r = self.operand(src, "t4")
                self.ins("neg", d, src_r)
            self.flush_dst(dst_var, d)
            return

        m = re.match(r'^(\w+)\s*=\s*-(\w+)$', instr)
        if m:
            dst_var, src = m.group(1), m.group(2)
            src = self._resolve_char_literal(src)
            d = self.dst_reg(dst_var)
            if is_int_literal(src):
                self.ins("li", d, str(-to_int(src)))
            else:
                src_r = self.operand(src, "t4")
                self.ins("neg", d, src_r)
            self.flush_dst(dst_var, d)
            return

        m = re.match(r'^(\w+)\s*=\s*~\s*(\w+)$', instr)
        if m:
            dst_var, src = m.group(1), m.group(2)
            d = self.dst_reg(dst_var)
            src_r = self.operand(src, "t4")
            self.ins("not", d, src_r)
            self.flush_dst(dst_var, d)
            return

        m = re.match(
            r'^(\w+)\s*=\s*(.+?)\s*(<=|>=|==|!=|<<|>>|[+\-*/%&|^<>])\s*(.+)$',
            instr)
        if m and m.group(2).strip():
            dst_var  = m.group(1)
            op1_raw  = m.group(2).strip()
            op       = m.group(3)
            op2_raw  = m.group(4).strip()

            def _is_float_val(v):
                if is_num_literal(v) and not is_int_literal(v):
                    return True
                if re.match(r'^[A-Za-z_]\w*$', v) and get_type(v) in ('float', 'double'):
                    return True
                if hasattr(self.ra_alloc, '_float_vars') and v in self.ra_alloc._float_vars:
                    return True
                return False

            def _load_fp_operand(v, fa_reg):
                if is_num_literal(v) and not is_int_literal(v):
                    lname = f".flt{len(self.flt_lits)}"
                    self.flt_lits.append((lname, v))
                    self.ins("la",  "t4", lname)
                    self.ins("fld", fa_reg, "0(t4)")
                    return fa_reg
                elif is_int_literal(v):
                    self.ins("li", "t4", to_int(v))
                    self.ins("fcvt.d.w", fa_reg, "t4")
                    return fa_reg
                else:
                    src_fp = self._is_fp_reg(v)
                    if src_fp:
                        return src_fp
                    elif self.ra_alloc.is_spilled(v):
                        src_is_fp = (
                            get_type(v) in ('float', 'double') or
                            (hasattr(self.ra_alloc, '_float_vars') and v in self.ra_alloc._float_vars)
                        )
                        if src_is_fp:
                            self.ins("fld", fa_reg, f"{self.ra_alloc.spill_offset(v)}(s0)")
                        else:
                            self.ins("lw", "t4", f"{self.ra_alloc.spill_offset(v)}(s0)")
                            self.ins("fcvt.d.w", fa_reg, "t4")
                        return fa_reg
                    else:
                        r = self.ra_alloc.reg(v)
                        src_is_fp = (
                            get_type(v) in ('float', 'double') or
                            (hasattr(self.ra_alloc, '_float_vars') and v in self.ra_alloc._float_vars)
                        )
                        if src_is_fp:
                            self.ins("fmv.d.x", fa_reg, r if r else "zero")
                        else:
                            self.ins("fcvt.d.w", fa_reg, r if r else "zero")
                        return fa_reg

            FLOAT_OPS = {'+': 'fadd.d', '-': 'fsub.d', '*': 'fmul.d', '/': 'fdiv.d'}
            op1_is_float = _is_float_val(op1_raw)
            op2_is_float = _is_float_val(op2_raw)

            if (op1_is_float or op2_is_float) and op in FLOAT_OPS:
                fop = FLOAT_OPS[op]
                op1_fp = self._is_fp_reg(op1_raw) if re.match(r'^[A-Za-z_]\w*$', op1_raw) else None
                op2_fp = self._is_fp_reg(op2_raw) if re.match(r'^[A-Za-z_]\w*$', op2_raw) else None
                if not hasattr(self.ra_alloc, '_float_vars'):
                    self.ra_alloc._float_vars = set()
                self.ra_alloc._float_vars.add(dst_var)
                set_type(dst_var, 'float')
                dst_fp = self._is_fp_reg(dst_var)

                r1 = _load_fp_operand(op1_raw, "fa0")
                r2 = _load_fp_operand(op2_raw, "fa1")

                if r1 == r2:
                    self.ins("fmv.d", "fa1", r2)
                    r2 = "fa1"

                out_reg = dst_fp if dst_fp and dst_fp != r2 else "fa0"
                self.ins(fop, out_reg, r1, r2)

                if not (dst_fp and out_reg == dst_fp):
                    self._store_float_result(out_reg, dst_var)
                return
            CMP_OPS = {'<', '>', '<=', '>=', '==', '!='}
            if op in CMP_OPS:
                if re.match(r'^(\w+)\[(\w+)\]$', op1_raw):
                    self._resolve_operand(op1_raw, "t4")
                    op1_reg = "t4"
                else:
                    op1_reg = self.var_reg_or_load(op1_raw, "t4")
                if re.match(r'^(\w+)\[(\w+)\]$', op2_raw):
                    self._resolve_operand(op2_raw, "t5")
                    op2_reg = "t5"
                elif is_num_literal(op2_raw):
                    self.ins("li", "t5", to_int(op2_raw))
                    op2_reg = "t5"
                else:
                    op2_reg = self.var_reg_or_load(op2_raw, "t5")
                if op1_reg == op2_reg and op1_reg in ("t4", "t5"):
                    self.load(op2_raw, "t5")
                    op2_reg = "t5"
                d = self.dst_reg(dst_var)
                if op == '<':
                    self.ins("slt",  f"{d}, {op1_reg}", op2_reg)
                elif op == '>':
                    self.ins("slt",  f"{d}, {op2_reg}", op1_reg)
                elif op == '<=':
                    self.ins("slt",  f"{d}, {op2_reg}", op1_reg)
                    self.ins("xori", f"{d}, {d}", 1)
                elif op == '>=':
                    self.ins("slt",  f"{d}, {op1_reg}", op2_reg)
                    self.ins("xori", f"{d}, {d}", 1)
                elif op == '==':
                    self.ins("sub",  f"{d}, {op1_reg}", op2_reg)
                    self.ins("seqz", f"{d}", d)
                elif op == '!=':
                    self.ins("sub",  f"{d}, {op1_reg}", op2_reg)
                    self.ins("snez", f"{d}", d)
                self.flush_dst(dst_var, d)
                return

            rr, ri   = self.BINOP_MAP.get(op, (None, None))
            if not rr:
                self.e(f"    # [UNSUPPORTED OP '{op}']")
                return

            if re.match(r'^(\w+)\[(\w+)\]$', op1_raw):
                self._resolve_operand(op1_raw, "t4")
                op1_reg = "t4"
            else:
                op1_reg = self.var_reg_or_load(op1_raw, "t4")

            d = self.dst_reg(dst_var)

            if is_num_literal(op2_raw) and ri:
                self.ins(ri, f"{d}, {op1_reg}", to_int(op2_raw))
            elif is_num_literal(op2_raw) and op == '-':
                self.ins("addi", f"{d}, {op1_reg}", -to_int(op2_raw))
            elif is_num_literal(op2_raw):
                self.ins("li", "t5", to_int(op2_raw))
                self.ins(rr, f"{d}, {op1_reg}", "t5")
            else:
                if re.match(r'^(\w+)\[(\w+)\]$', op2_raw):
                    self._resolve_operand(op2_raw, "t5")
                    op2_reg = "t5"
                else:
                    op2_reg = self.var_reg_or_load(op2_raw, "t5")
                if op1_reg == op2_reg and op1_reg in ("t4", "t5"):
                    self.load(op2_raw, "t5")
                    op2_reg = "t5"
                self.ins(rr, f"{d}, {op1_reg}", op2_reg)
            self.flush_dst(dst_var, d)
            return

        m = re.match(r'^(\w+)\s*=\s*(\S+)$', instr)
        if m:
            dst_var, val = m.group(1), m.group(2)
            val = self._resolve_char_literal(val)

            src_is_float = (
                (is_num_literal(val) and not is_int_literal(val)) or
                (re.match(r'^[A-Za-z_]\w*$', val) and get_type(val) in ('float', 'double')) or
                (hasattr(self.ra_alloc, '_float_vars') and val in self.ra_alloc._float_vars)
            )
            dst_is_float = (
                get_type(dst_var) in ('float', 'double') or
                (hasattr(self.ra_alloc, '_float_vars') and dst_var in self.ra_alloc._float_vars)
            )

            if src_is_float or dst_is_float:
                if not hasattr(self.ra_alloc, '_float_vars'):
                    self.ra_alloc._float_vars = set()
                self.ra_alloc._float_vars.add(dst_var)
                set_type(dst_var, get_type(val) if re.match(r'^[A-Za-z_]\w*$', val) else 'float')

                dst_fp = self._is_fp_reg(dst_var)
                tgt = dst_fp if dst_fp else "fa0"

                if is_num_literal(val) and not is_int_literal(val):
                    lname = f".flt{len(self.flt_lits)}"
                    self.flt_lits.append((lname, val))
                    self.ins("la",  "t4", lname)
                    self.ins("fld", tgt, "0(t4)")
                elif is_int_literal(val):
                    self.ins("li", "t4", to_int(val))
                    self.ins("fcvt.d.w", tgt, "t4")
                elif self.ra_alloc.is_spilled(val):
                    val_is_fp = (
                        get_type(val) in ('float', 'double') or
                        (hasattr(self.ra_alloc, '_float_vars') and val in self.ra_alloc._float_vars)
                    )
                    if val_is_fp:
                        self.ins("fld", tgt, f"{self.ra_alloc.spill_offset(val)}(s0)")
                    else:
                        self.ins("lw", "t4", f"{self.ra_alloc.spill_offset(val)}(s0)")
                        self.ins("fcvt.d.w", tgt, "t4")
                else:
                    src_fp = self._is_fp_reg(val)
                    if src_fp:
                        if src_fp != tgt:
                            self.ins("fmv.d", tgt, src_fp)
                    else:
                        r = self.ra_alloc.reg(val)
                        val_is_fp = (
                            get_type(val) in ('float', 'double') or
                            (hasattr(self.ra_alloc, '_float_vars') and val in self.ra_alloc._float_vars)
                        )
                        if val_is_fp:
                            self.ins("fmv.d.x", tgt, r if r else "zero")
                        else:
                            self.ins("fcvt.d.w", tgt, r if r else "zero")

                if not dst_fp:
                    self._store_float_result("fa0", dst_var)
            else:
                d = self.dst_reg(dst_var)
                if is_int_literal(val):
                    self.ins("li", d, to_int(val))
                else:
                    src_r = self.operand(val, "t4")
                    if src_r != d:
                        self.ins("mv", d, src_r)
                self.flush_dst(dst_var, d)
            return

        self.e(f"    # [UNHANDLED] {instr}")

def main():
    import argparse
    import subprocess
    import shutil
    import tempfile
    import os

    ap = argparse.ArgumentParser(description="Convert TAC to RISC-V assembly (v6)")
    ap.add_argument("input",   nargs="?", default="output.tac")
    ap.add_argument("output",  nargs="?", default="output.s")
    ap.add_argument("--run",   action="store_true",
                    help="Compile the generated .s and run it immediately")
    ap.add_argument("--compiler", default=None,
                    help="Override GCC cross-compiler (e.g. riscv64-linux-gnu-gcc)")
    ap.add_argument("--emulator", default=None,
                    help="Override QEMU emulator (e.g. qemu-riscv64)")
    args = ap.parse_args()

    if not os.path.exists(args.input):
      return

    with open(args.input) as f:
      tac = f.read()

    asm = TACtoRISCV().convert(tac)

    with open(args.output, "w") as f:
        f.write(asm)

    lines    = asm.splitlines()
    funcs    = [l for l in lines if re.match(r'^\w.*:$', l)]
    branches = [l for l in lines if re.search(r'\b(beq|bne|blt|bge|j )', l)]
    print(f"[OK] {args.input}  ->  {args.output}")
    print(f"     Functions : {len(funcs)}")
    print(f"     Branches  : {len(branches)}")
    print(f"     Lines     : {len(lines)}")

    print(f"\n{'='*40}")
    print(f"[ASM] {args.output}")
    print('='*40)
    print(asm)
    print('='*40)

    if not args.run:
        return

    COMPILER_CANDIDATES = [
        ("riscv64-linux-gnu-gcc",        "qemu-riscv64"),
        ("riscv64-unknown-linux-gnu-gcc", "qemu-riscv64"),
        ("riscv32-linux-gnu-gcc",        "qemu-riscv32"),
        ("riscv32-unknown-linux-gnu-gcc","qemu-riscv32"),
    ]

    compiler = args.compiler
    emulator = args.emulator

    if not compiler:
        for cc, qemu in COMPILER_CANDIDATES:
            if shutil.which(cc):
                compiler = cc
                if not emulator:
                    emulator = qemu
                break

    if not compiler:
        print("\n[ERROR] No RISC-V cross-compiler found on PATH.", file=sys.stderr)
        print("  Install one, e.g.:", file=sys.stderr)
        print("    sudo apt install gcc-riscv64-linux-gnu qemu-user", file=sys.stderr)
        print("  Or pass --compiler /path/to/riscv64-linux-gnu-gcc", file=sys.stderr)
        sys.exit(1)

    if not emulator:
        emulator = "qemu-riscv64"

    with tempfile.TemporaryDirectory() as tmpdir:
        bin_path = os.path.join(tmpdir, "a.out")
        compile_cmd = [compiler, "-static", "-o", bin_path, args.output]
        print(f"\n[COMPILE] {' '.join(compile_cmd)}")
        result = subprocess.run(compile_cmd, capture_output=True, text=True)
        if result.returncode != 0:
            print("[COMPILE FAILED]", file=sys.stderr)
            print(result.stderr, file=sys.stderr)
            sys.exit(result.returncode)
        print("[COMPILE OK]")

        run_cmd = None
        if shutil.which(emulator):
            run_cmd = [emulator, bin_path]
        else:
            run_cmd = [bin_path]

        print(f"[RUN]     {' '.join(run_cmd)}")
        print("-" * 40)
        run_result = subprocess.run(run_cmd, text=True)
        print("-" * 40)
        print(f"[EXIT CODE] {run_result.returncode}")

if __name__ == "__main__":
    main()