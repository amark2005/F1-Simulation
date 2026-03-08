import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("race.csv")

teams = df["teams"].unique()

plt.figure()

for team in teams:
    team_data = df[df["teams"] == team]
    plt.plot(team_data["lap"], team_data["laptime"], label=team)

plt.xlabel("Lap")
plt.ylabel("Lap Time (s)")
plt.title("F1 Monaco Lap Times")
plt.legend()
plt.grid(True)


plt.show()