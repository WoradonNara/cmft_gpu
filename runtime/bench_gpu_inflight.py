#!/usr/bin/env python3
import argparse
import pathlib
import re
import statistics
import subprocess
import sys


TOTAL_TIME_RE = re.compile(r"Total time:\s*([0-9.]+)\s*seconds")
DEPTH_RE = re.compile(r"GPU in-flight task depth:\s*(\d+)")


def run_once(exe_path, args, depth, run_idx):
    cmd = [str(exe_path)] + args + ["--gpuInFlightTasks", str(depth)]
    proc = subprocess.run(cmd, capture_output=True, text=True)
    output = proc.stdout + "\n" + proc.stderr

    if proc.returncode != 0:
        tail = output[-1200:]
        raise RuntimeError(
            "DEPTH={} RUN={} failed (rc={})\n{}".format(depth, run_idx, proc.returncode, tail)
        )

    total_match = TOTAL_TIME_RE.search(output)
    if total_match is None:
        tail = output[-1200:]
        raise RuntimeError(
            "DEPTH={} RUN={} did not report total time\n{}".format(depth, run_idx, tail)
        )

    total_seconds = float(total_match.group(1))
    depth_match = DEPTH_RE.search(output)
    actual_depth = int(depth_match.group(1)) if depth_match else None

    return total_seconds, actual_depth


def build_cmft_args(ns, out_name):
    return [
        "--input", ns.input,
        "--filter", "radiance",
        "--dstFaceSize", str(ns.dst_face_size),
        "--srcFaceSize", str(ns.src_face_size),
        "--excludeBase", str(ns.exclude_base).lower(),
        "--mipCount", str(ns.mip_count),
        "--glossScale", str(ns.gloss_scale),
        "--glossBias", str(ns.gloss_bias),
        "--lightingModel", ns.lighting_model,
        "--numCpuProcessingThreads", str(ns.num_cpu_threads),
        "--processingMode", "gpuOnly",
        "--useOpenCL", "true",
        "--clVendor", ns.cl_vendor,
        "--deviceType", ns.device_type,
        "--deviceIndex", str(ns.device_index),
        "--generateMipChain", "false",
        "--outputNum", "1",
        "--output0", out_name,
        "--output0params", "dds,bgra8,cubemap",
    ]


def main():
    parser = argparse.ArgumentParser(description="Benchmark cmft GPU in-flight task depth.")
    parser.add_argument(
        "--exe",
        default="_build/win64_vs2015/bin/cmftRelease.exe",
        help="Path to cmft executable.",
    )
    parser.add_argument("--input", required=True, help="Input environment map path.")
    parser.add_argument("--runs", type=int, default=3, help="Runs per depth.")
    parser.add_argument("--depths", nargs="+", type=int, default=[0, 1, 2], help="Depth values to test.")
    parser.add_argument("--src-face-size", type=int, default=512)
    parser.add_argument("--dst-face-size", type=int, default=256)
    parser.add_argument("--mip-count", type=int, default=1)
    parser.add_argument("--gloss-scale", type=int, default=10)
    parser.add_argument("--gloss-bias", type=int, default=1)
    parser.add_argument("--lighting-model", default="phong")
    parser.add_argument("--exclude-base", action="store_true")
    parser.add_argument("--num-cpu-threads", type=int, default=0)
    parser.add_argument("--cl-vendor", default="anyGpuVendor")
    parser.add_argument("--device-type", default="gpu")
    parser.add_argument("--device-index", type=int, default=0)
    parser.add_argument("--out-prefix", default="runtime/bench_inflight")
    ns = parser.parse_args()

    if ns.runs < 1:
        print("--runs must be >= 1", file=sys.stderr)
        return 2

    exe_path = pathlib.Path(ns.exe)
    if not exe_path.exists():
        print("cmft executable not found: {}".format(exe_path), file=sys.stderr)
        return 2

    summary_rows = []
    for depth in ns.depths:
        out_name = "{}_d{}".format(ns.out_prefix, depth)
        args = build_cmft_args(ns, out_name)
        timings = []

        for run_idx in range(1, ns.runs + 1):
            total_s, actual_depth = run_once(exe_path, args, depth, run_idx)
            timings.append(total_s)
            print(
                "DEPTH={} RUN={} actualDepth={} total={:.3f}s".format(
                    depth,
                    run_idx,
                    actual_depth if actual_depth is not None else "n/a",
                    total_s,
                )
            )

        avg_s = statistics.mean(timings)
        summary_rows.append((depth, len(timings), avg_s, min(timings), max(timings)))

    print("\nRESULTS")
    for depth, n, avg_s, min_s, max_s in summary_rows:
        print("depth={} n={} avg={:.3f}s min={:.3f}s max={:.3f}s".format(depth, n, avg_s, min_s, max_s))

    best = min(summary_rows, key=lambda row: row[2])
    print("\nBEST depth={} avg={:.3f}s".format(best[0], best[2]))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
