import asyncio
import argparse
import typing
import json


def parse_wrk_output(text: str) -> typing.Tuple[float, float]:
    req_sec = -1
    transfer_sec = -1
    for line in text.splitlines():
        if line.startswith("Requests/sec:"):
            req_sec = line.replace("Requests/sec:", "").strip()
        elif line.startswith("Transfer/sec:"):
            transfer_sec = line.replace("Transfer/sec:", "").strip()
    if transfer_sec.endswith("MB"):
        transfer_sec = float(transfer_sec[:-2])
    elif transfer_sec.endswith("GB"):
        transfer_sec = float(transfer_sec[:-2]) * 1024
    elif transfer_sec.endswith("KB"):
        transfer_sec = float(transfer_sec[:-2]) / 1024
    return float(req_sec), float(transfer_sec)


def handle_sequence(seq: typing.List[float]) -> dict:
    avg = sum(seq) / len(seq)
    sqr_sum = sum(x**2 for x in seq) / len(seq)

    result = {
        "max": max(seq),
        "min": min(seq),
        "avg": avg,
        "sqr_dev": sqr_sum - avg**2,
        "std_dev": (sqr_sum - avg**2) ** 0.5,
    }
    return {"raw": seq, "statistics": result}


async def main():
    args = argparse.ArgumentParser()
    args.add_argument("URL", action="store", help="URL to test")
    args.add_argument("COUNT", action="store", type=int)
    args.add_argument("-c", "--connection", action="store", type=int, default=250)
    args.add_argument("-t", "--thread", action="store", type=int, default=20)
    args.add_argument("OUTPUT", action="store", default="benchmark.json")
    args = args.parse_args()
    result = {"req": [], "transfer": []}
    for i in range(1, args.COUNT + 1):
        print(f"Time: {i}")
        wrk = await asyncio.create_subprocess_exec(
            "wrk",
            "-c",
            str(args.connection),
            "-t",
            str(args.thread),
            args.URL,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.STDOUT,
        )
        stdout, _ = await wrk.communicate()
        await wrk.wait()
        req, trans = parse_wrk_output(stdout.decode())
        result["req"].append(req)
        result["transfer"].append(trans)
        print(stdout.decode())
    result["req"] = handle_sequence(result["req"])
    result["transfer"] = handle_sequence(result["transfer"])
    with open(args.OUTPUT, "w") as f:
        json.dump(result, f)


if __name__ == "__main__":
    asyncio.run(main())
