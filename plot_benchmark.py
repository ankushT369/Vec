# plot_benchmark.py
import matplotlib.pyplot as plt

sizes = []
custom_times = []
std_times = []

with open("benchmark.out") as f:
    for line in f:
        if line.startswith("#") or not line.strip():
            continue
        parts = line.strip().split()
        if len(parts) != 3:
            continue
        N, custom, std = parts
        sizes.append(int(N))
        custom_times.append(float(custom))
        std_times.append(float(std))

if not sizes:
    print("No valid benchmark data found.")
    exit(1)

plt.figure(figsize=(8, 5))
plt.plot(sizes, custom_times, label="Custom Vec<int>", marker='o', color='#66c2a5')
plt.plot(sizes, std_times, label="std::vector<int>", marker='s', color='#fc8d62')

plt.xscale("log")  # optional, to better visualize large ranges
plt.title("Benchmark: Custom Vec vs std::vector")
plt.xlabel("Number of Elements (log scale)")
plt.ylabel("Time (ms)")
plt.legend()
plt.grid(True, which="both", linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig("benchmark.png")
print("Saved benchmark.png")

