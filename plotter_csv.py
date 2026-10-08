import pandas as pd
import matplotlib.pyplot as plt

bucket = pd.read_csv("cfl_Bucket.csv")
barrier = pd.read_csv("cfl_Barrier.csv")
platforms = pd.read_csv("cfl_Platforms.csv")

plt.plot(
    bucket["time"],
    bucket["cfl"],
    label="Bucket"
)

plt.plot(
    barrier["time"],
    barrier["cfl"],
    label="Barrier"
)

plt.plot(
    platforms["time"],
    platforms["cfl"],
    label="Platforms"
)

plt.xlabel("Simulation Time (s)")
plt.ylabel("CFL Number")
plt.title("CFL Number Over Time")

plt.legend()
plt.grid(True)
plt.tight_layout()

plt.savefig("cfl.png", dpi=300)
