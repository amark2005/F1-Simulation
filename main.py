import os
import platform
import subprocess
import sys
import pandas as pd
import matplotlib.pyplot as plt

system = platform.system()

compile_cmd = "g++ -std=c++20 -O3 -Wall -Wextra -pthread engine.cpp -o engine"
binary = "./engine" if os.name != "nt" else "engine.exe"

if not os.path.exists(binary):
    print("Binary not found. Compiling engine.cpp...")
    result = subprocess.run(compile_cmd, shell=True)

    if result.returncode != 0:
        print("Compilation failed.")
        sys.exit(1)

print("Running race simulator...")
subprocess.run(binary, shell=True)

if not os.path.exists("race.csv"):
    print("race.csv not generated.")
    sys.exit(1)

df = pd.read_csv("race.csv")
df = df.sort_values(["teams","lap"])

df["total"] = df.groupby("teams")["laptime"].cumsum()
leader_time = df.groupby("lap")["total"].transform("min")
df["gap"] = df["total"] - leader_time

teams = df["teams"].unique()

fig, axes = plt.subplots(4,1,figsize=(10,16))

for team in teams:
    d = df[df["teams"]==team]
    axes[0].plot(d["lap"],d["laptime"],label=team)

axes[0].set_title("Lap Times")
axes[0].set_ylabel("Lap Time (s)")
axes[0].legend()
axes[0].grid(True)

for team in teams:
    d = df[df["teams"]==team]
    axes[1].plot(d["lap"],d["total"],label=team)

axes[1].set_title("Race Progression")
axes[1].set_ylabel("Total Time (s)")
axes[1].legend()
axes[1].grid(True)

for team in teams:
    d = df[df["teams"]==team]
    axes[2].plot(d["lap"],d["gap"],label=team)

axes[2].set_title("Gap to Leader")
axes[2].set_ylabel("Gap (s)")
axes[2].legend()
axes[2].grid(True)

for team in teams:
    d = df[df["teams"]==team]
    axes[3].plot(d["lap"],d["tyre_wear"],label=team)

axes[3].set_title("Tyre Degradation")
axes[3].set_xlabel("Lap")
axes[3].set_ylabel("Tyre Wear")
axes[3].legend()
axes[3].grid(True)

plt.tight_layout(pad=3.0) 
plt.show()

