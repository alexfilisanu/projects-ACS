import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Get df from saved csv
weather_df = pd.read_csv("bucharest-weather.csv")

# Distribution of weather values
plt.figure(figsize=(8, 6))
sns.countplot(data=weather_df, x='weather', hue='weather', palette='Set1')
plt.title('Distribution of weather values')
plt.xlabel('Weather')
plt.ylabel('Frequency')
plt.show()

# Distribution of precipitation type
plt.figure(figsize=(8, 6))
sns.countplot(data=weather_df, x='preciptype', hue='preciptype', palette='Set2')
plt.title('Distribution of precipitation type')
plt.xlabel('Precipitation Type')
plt.ylabel('Frequency')
plt.show()

# Precipitation histogram
plt.figure(figsize=(8, 6))
sns.histplot(data=weather_df, x="precip", kde=True, color='green')
plt.title('Distribution of Precipitation')
plt.xlabel('Precipitation')
plt.ylabel('Frequency')
plt.show()

# Max Temperature histogram
plt.figure(figsize=(8, 6))
sns.histplot(data=weather_df, x="tempmax", kde=True, color='red')
plt.title('Distribution of Max Temperature')
plt.xlabel('Max Temperature')
plt.ylabel('Frequency')
plt.show()

# Min Temperature histogram
plt.figure(figsize=(8, 6))
sns.histplot(data=weather_df, x="tempmin", kde=True, color='skyblue')
plt.title('Distribution of Min Temperature')
plt.xlabel('Min Temperature')
plt.ylabel('Frequency')
plt.show()

# Windspeed histogram
plt.figure(figsize=(8, 6))
sns.histplot(data=weather_df, x="windspeed", kde=True, color='orange')
plt.title('Distribution of Windspeed')
plt.xlabel('Windspeed')
plt.ylabel('Frequency')
plt.show()

# Violin plots for weather attributes for outlier detection
fig, axs = plt.subplots(2, 2, figsize=(10, 8))

# Violin plot for Precipitation
sns.violinplot(data=weather_df, x="precip", ax=axs[0, 0], color='green')
axs[0, 0].set_title('Distribution of Precipitation')
axs[0, 0].set_xlabel('Precipitation')
axs[0, 0].set_ylabel('Density')

# Violin plot for Max Temperature
sns.violinplot(data=weather_df, x="tempmax", ax=axs[0, 1], color='red')
axs[0, 1].set_title('Distribution of Max Temperature')
axs[0, 1].set_xlabel('Max Temperature')
axs[0, 1].set_ylabel('Density')

# Violin plot for Min Temperature
sns.violinplot(data=weather_df, x="tempmin", ax=axs[1, 0], color='skyblue')
axs[1, 0].set_title('Distribution of Min Temperature')
axs[1, 0].set_xlabel('Min Temperature')
axs[1, 0].set_ylabel('Density')

# Violin plot for Windspeed
sns.violinplot(data=weather_df, x="windspeed", ax=axs[1, 1], color='yellow')
axs[1, 1].set_title('Distribution of Windspeed')
axs[1, 1].set_xlabel('Windspeed')
axs[1, 1].set_ylabel('Density')

plt.tight_layout()
plt.show()

# Normalize the data
weather_df['precip'] = np.sqrt(weather_df['precip'])
weather_df['windspeed'] = np.sqrt(weather_df['windspeed'])

# Precipitation histogram
plt.figure(figsize=(8, 6))
sns.histplot(data=weather_df, x="precip", kde=True, color='green')
plt.title('Distribution of Precipitation - Normalized')
plt.xlabel('Precipitation')
plt.ylabel('Frequency')
plt.show()

# Windspeed histogram
plt.figure(figsize=(8, 6))
sns.histplot(data=weather_df, x="windspeed", kde=True, color='orange')
plt.title('Distribution of Windspeed - Normalized')
plt.xlabel('Windspeed')
plt.ylabel('Frequency')
plt.show()

# Save the normalized data
weather_df.to_csv("bucharest-weather-normalized.csv", index=False)
