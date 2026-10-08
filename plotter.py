import pandas as pd
import matplotlib.pyplot as plt

bucket = pd.read_csv("density_Bucket.csv")
barrier = pd.read_csv("density_Barrier.csv")
funnel = pd.read_csv("density_Platforms.csv")

plt.plot(
    bucket["time"],
    bucket["average_density"],
    label="Bucket"
)

plt.plot(
    barrier["time"],
    barrier["average_density"],
    label="Barrier"
)

plt.plot(
    funnel["time"],
    funnel["average_density"],
    label="Platforms"
)

plt.axhline(
    y=1.1,
    linestyle="--",
    label="Rest density"
)

plt.xlabel("Simulation Time (s)")
plt.ylabel("Average Density")
plt.title("Average Fluid Density Over Time")

plt.legend()
plt.grid(True)
plt.tight_layout()

plt.savefig("average_density.png", dpi=300)
