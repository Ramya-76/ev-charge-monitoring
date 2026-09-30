"""
Analyze charging data exported from Blynk (CSV).
Rename the column names below to match your Blynk CSV export.
Expected columns: created_at, voltage, current, power, energy
Usage: python analyze_data.py data.csv
"""
import sys
import pandas as pd
import matplotlib.pyplot as plt

if len(sys.argv) < 2:
    sys.exit("Usage: python analyze_data.py data.csv")

df = pd.read_csv(sys.argv[1], parse_dates=["created_at"])

print("Samples:", len(df))
print("Average voltage (V): %.2f" % df["voltage"].mean())
print("Peak current (A):    %.2f" % df["current"].max())
print("Peak power (W):      %.2f" % df["power"].max())
print("Total energy (Wh):   %.3f" % df["energy"].iloc[-1])

fig, axes = plt.subplots(3, 1, figsize=(9, 8), sharex=True)
axes[0].plot(df["created_at"], df["voltage"]); axes[0].set_ylabel("Voltage (V)")
axes[1].plot(df["created_at"], df["current"], color="tab:orange"); axes[1].set_ylabel("Current (A)")
axes[2].plot(df["created_at"], df["energy"], color="tab:green"); axes[2].set_ylabel("Energy (Wh)")
axes[2].set_xlabel("Time")
plt.tight_layout()
plt.savefig("charging_plot.png", dpi=150)
plt.show()
