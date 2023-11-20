for c in ["--http", "--https"]:
    for a in ["no-probe", "kernel-uprobe", "no-uprobe", "user-uprobe"]:
        for b in range(1, 11):
            import os

            print(a, b, c)
            os.system(f"python3 run.py -t {a} {c} > test-{a}-{b}_{c}.txt")
