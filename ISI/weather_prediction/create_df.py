from datetime import datetime, timedelta
import requests
import pandas as pd

# file_paths = [
#     "Bucharest,Romania 2014-12-01 to 2016-11-30.csv",
#     "Bucharest,Romania 2016-12-01 to 2018-11-30.csv",
#     "Bucharest,Romania 2018-12-01 to 2020-11-30.csv",
#     "Bucharest,Romania 2020-12-01 to 2022-11-30.csv",
#     "Bucharest,Romania 2022-12-01 to 2024-12-01.csv"
# ]
#
# dataframes = [pd.read_csv(file_path) for file_path in file_paths]
# weather_df = pd.concat(dataframes, ignore_index=True)
# weather_df["preciptype"] = weather_df.apply(
#     lambda row: "snow" if row["snow"] != 0 else ("rain" if "," in str(row["preciptype"]) else row["preciptype"]),
#     axis=1,
# )
# weather_df = weather_df[["datetime", "tempmax", "tempmin", "precip", "preciptype", "windspeed", "cloudcover", "icon"]]
# weather_df['preciptype'] = weather_df['preciptype'].fillna("no")
# weather_df.rename(columns={"icon": "weather"}, inplace=True)
#
# # Save the data
# weather_df.to_csv("bucharest-weather.csv", index=False)

API_URL = "https://weather.visualcrossing.com/VisualCrossingWebServices/rest/services/timeline"
API_KEY = "VQMZ7NRBJCG3BX49Y4K6ZMMTW"


# Funcție pentru a obține datele meteo
def get_weather_data(location, start_date, end_date):
    url = f"{API_URL}/{location}/{start_date}/{end_date}"
    params = {
        "unitGroup": "metric",
        "include": "days",
        "key": API_KEY,
        "contentType": "json",
    }
    response = requests.get(url, params=params)
    response.raise_for_status()
    data = response.json()
    return data["days"]


# Funcție pentru a procesa datele meteo
def process_weather_data(data):
    return [
        {
            "datetime": day["datetime"],
            "tempmax": day["tempmax"],
            "tempmin": day["tempmin"],
            "precip": day.get("precip", 0),
            "preciptype": day.get("preciptype", ["no"])[0] if day.get("preciptype") else "no",
            "windspeed": day["windspeed"],
            "cloudcover": day["cloudcover"],
            "weather": day["icon"]
        }
        for day in data
    ]


# Funcție pentru actualizarea fișierului CSV
def update_weather_data(location, csv_file):
    try:
        weather_df = pd.read_csv(csv_file)
        last_date = pd.to_datetime(weather_df["datetime"].max()).date()
    except FileNotFoundError:
        weather_df = pd.DataFrame()
        last_date = datetime.now().date() - timedelta(
            days=730)  # Dacă nu există fișierul, preia date din ultimii 2 ani.

    today = datetime.now().date()
    if last_date < today:
        start_date = (last_date + timedelta(days=1)).strftime("%Y-%m-%d")
        end_date = today.strftime("%Y-%m-%d")

        # Obține noile date meteo
        print(f"Obțin date meteo de la {start_date} până la {end_date}...")
        data = get_weather_data(location, start_date, end_date)
        new_data = process_weather_data(data)
        new_df = pd.DataFrame(new_data)

        # Adaugă noile date la datasetul existent
        weather_df = pd.concat([weather_df, new_df], ignore_index=True)
        weather_df.to_csv(csv_file, index=False)
        print(f"Datele meteo au fost actualizate până la {end_date}.")
    else:
        print("Datele sunt deja la zi.")


# Exemplu de utilizare
update_weather_data("bucharest", "bucharest-weather.csv")