# plot_benchmark.py
import matplotlib.pyplot as plt

custom_time = None
std_time = None

with open("benchmark.out") as f:
    for line in f:
        if "Custom Vec<int>" in line:
            custom_time = float(line.strip().split()[-1])
        if "std::vector<int>" in line:
            std_time = float(line.strip().split()[-1])

if custom_time is None or std_time is None:
    print("Benchmark results not found.")
    exit(1)

# Plot
labels = ['Custom Vec<int>', 'std::vector<int>']
values = [custom_time, std_time]

plt.figure(figsize=(6, 4))
plt.bar(labels, values, color=['#66c2a5', '#fc8d62'])
plt.title("Benchmark Time (ms)")
plt.ylabel("Time (ms)")
plt.tight_layout()
plt.savefig("benchmark.png")
print("Saved benchmark.png")

