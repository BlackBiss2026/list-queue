#!/usr/bin/env python3
"""
Checks the exercise's main rule on src/list.c and src/queue.c:
  * no loops (for / while / do), no goto  -- except inside list_destroy()
  * no recursion (direct or indirect)
  * no setjmp/longjmp tricks, no loop keywords hidden in macros

Usage: python3 tests/check_rules.py [files...]
Exit code 0 = OK, 1 = rule broken.
"""
import re
import sys

FILES = sys.argv[1:] or ["src/list.c", "src/queue.c"]
LOOP_ALLOWED_IN = {"list_destroy"}
FORBIDDEN = re.compile(r"\b(for|while|do|goto)\b")
JUMPS = re.compile(r"\b(setjmp|longjmp|sigsetjmp|siglongjmp)\b")
KEYWORDS = {"if", "for", "while", "do", "switch", "return", "sizeof", "else"}


def strip(code):
    """Remove comments, string and char literals (keep newlines)."""
    out, i, n = [], 0, len(code)
    while i < n:
        c = code[i]
        if code.startswith("//", i):
            j = code.find("\n", i)
            i = n if j < 0 else j
        elif code.startswith("/*", i):
            j = code.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append("\n" * code.count("\n", i, j))
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and code[j] != c:
                j += 2 if code[j] == "\\" else 1
            out.append(c + c)
            i = j + 1
        else:
            out.append(c)
            i += 1
    return "".join(out)


def functions(code):
    """Yield (name, body, line) for every top-level function definition."""
    depth, start, header_start = 0, None, 0
    for i, c in enumerate(code):
        if c == "{":
            if depth == 0:
                header = code[header_start:i]
                m = re.search(r"([A-Za-z_]\w*)\s*\([^()]*\)\s*$", header)
                start = (m.group(1) if m else None, i)
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0 and start:
                name, s = start
                if name:
                    yield name, code[s:i + 1], code.count("\n", 0, s) + 1
                start = None
                header_start = i + 1
        elif c == ";" and depth == 0:
            header_start = i + 1


def main():
    errors = []
    defs = {}
    for path in FILES:
        try:
            with open(path, encoding="utf-8", errors="replace") as f:
                code = strip(f.read())
        except OSError as e:
            errors.append(f"{path}: cannot read ({e})")
            continue
        for lineno, line in enumerate(code.splitlines(), 1):
            if line.lstrip().startswith("#") and FORBIDDEN.search(line):
                errors.append(f"{path}:{lineno}: loop/goto keyword inside a macro")
            if JUMPS.search(line):
                errors.append(f"{path}:{lineno}: setjmp/longjmp is not allowed")
        for name, body, line in functions(code):
            defs[name] = (path, body, line)
            if name not in LOOP_ALLOWED_IN:
                for m in FORBIDDEN.finditer(body):
                    ln = line + body.count("\n", 0, m.start())
                    errors.append(f"{path}:{ln}: '{m.group(1)}' used in {name}() "
                                  f"(loops/goto allowed only in list_destroy)")

    # call graph between functions defined in these files
    calls = {}
    for name, (_, body, _) in defs.items():
        called = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", body)) - KEYWORDS
        calls[name] = {c for c in called if c in defs}

    # also catch a function's address being taken and called indirectly
    for name, (_, body, _) in defs.items():
        for other in defs:
            if re.search(r"\b%s\b(?!\s*\()" % re.escape(other), body):
                calls[name].add(other)

    seen_cycles = set()

    def dfs(node, stack):
        for nxt in calls.get(node, ()):
            if nxt in stack:
                cyc = stack[stack.index(nxt):] + [nxt]
                key = frozenset(cyc)
                if key not in seen_cycles:
                    seen_cycles.add(key)
                    errors.append("recursion: " + " -> ".join(n + "()" for n in cyc))
            elif len(stack) < 50:
                dfs(nxt, stack + [nxt])

    for f in defs:
        dfs(f, [f])

    if errors:
        print("RULE CHECK FAILED:")
        for e in errors:
            print("  " + e)
        return 1
    print(f"Rule check passed ({len(defs)} functions checked: no loops outside "
          "list_destroy, no goto, no recursion).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
