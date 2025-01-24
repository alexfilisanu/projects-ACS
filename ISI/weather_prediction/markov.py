import pandas as pd
from hmmlearn import hmm
from sklearn.metrics import accuracy_score, classification_report
from sklearn.preprocessing import LabelEncoder, StandardScaler, MinMaxScaler, RobustScaler

weather_df = pd.read_csv("bucharest-weather-normalized.csv")
weather_df['datetime'] = pd.to_datetime(weather_df['datetime'])

weather_df['day'] = weather_df['datetime'].dt.day
weather_df['month'] = weather_df['datetime'].dt.month

# Encoding categorical variables
label_encoder_precip = LabelEncoder()
weather_df['preciptype_encoded'] = label_encoder_precip.fit_transform(weather_df['preciptype'])
label_encoder_weather = LabelEncoder()
weather_df['weather_encoded'] = label_encoder_weather.fit_transform(weather_df['weather'])

# Splitting the data into features and target
X = weather_df[['tempmax', 'tempmin', 'precip', 'preciptype_encoded', 'windspeed', 'cloudcover', 'day', 'month']]
Y = weather_df['weather_encoded'].values

# Scaling the features
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# Discretizarea caracteristicilor continue
weather_df['tempmax_bin'] = pd.cut(weather_df['tempmax'], bins=4, labels=False)
weather_df['tempmin_bin'] = pd.cut(weather_df['tempmin'], bins=4, labels=False)
weather_df['precip_bin'] = pd.cut(weather_df['precip'], bins=4, labels=False)
weather_df['windspeed_bin'] = pd.cut(weather_df['windspeed'], bins=4, labels=False)
weather_df['cloudcover_bin'] = pd.cut(weather_df['cloudcover'], bins=4, labels=False)
weather_df['day_bin'] = pd.cut(weather_df['day'], bins=4, labels=False)
weather_df['month_bin'] = pd.cut(weather_df['month'], bins=4, labels=False)

X_discrete = weather_df[['tempmax_bin', 'tempmin_bin', 'precip_bin', 'preciptype_encoded', 'windspeed_bin', 'cloudcover_bin', 'day_bin', 'month_bin']]


def test_hmm(X_data, Y_data, n_states_range, use_discrete=False):
    best_accuracy = 0
    best_n_states = 0
    best_model = None

    for n_states in n_states_range:
        print(f"Testăm modelul cu {n_states} stări ascunse...")
        model = hmm.MultinomialHMM(n_components=n_states, n_iter=100, random_state=42) if use_discrete else \
            hmm.GaussianHMM(n_components=n_states, covariance_type="diag", n_iter=100, random_state=42)

        model.fit(X_data)
        predicted_weather = model.predict_proba(X_data).argmax(axis=1)

        # Calcularea acurateței
        accuracy = accuracy_score(Y_data.flatten(), predicted_weather)
        print(f"Acuratețea pentru {n_states} stări: {accuracy}")

        if accuracy > best_accuracy:
            best_accuracy = accuracy
            best_n_states = n_states
            best_model = model

    print(f"\nCel mai bun model are {best_n_states} stări ascunse cu o acuratețe de {best_accuracy}")
    # print(classification_report(Y_data.flatten(), best_model.predict(X_data)))
    return best_model, best_accuracy, best_n_states


# Testarea cu date continue (GaussianHMM)
print("\nTestare cu date continue:")
best_model_gaussian, best_accuracy_gaussian, best_n_states_gaussian = test_hmm(X_scaled, Y, range(2, 10))

# Testarea cu date discrete (MultinomialHMM)
print("\nTestare cu date discrete:")
best_model_discrete, best_accuracy_discrete, best_n_states_discrete = test_hmm(X_discrete, Y, range(2, 10),
                                                                               use_discrete=True)

# Matricea de tranziție pentru modelul cel mai bun (continuă)
print("\nMatricea de tranziție pentru modelul GaussianHMM cel mai bun:")
print(best_model_gaussian.transmat_)

# Matricea de tranziție pentru modelul cel mai bun (discret)
print("\nMatricea de tranziție pentru modelul MultinomialHMM cel mai bun:")
print(best_model_discrete.transmat_)

# Parametrii distribuțiilor pentru modelul Gaussian
print("\nParametrii distribuțiilor pentru modelul GaussianHMM cel mai bun:")
for i in range(best_n_states_gaussian):
    print(f"Stare {i}: Mean = {best_model_gaussian.means_[i]}, Covariance = {best_model_gaussian.covars_[i]}")

# Exemplu de prezicție pentru o zi
example_day = X_scaled[0].reshape(1, -1)  # Prima zi din date (pentru model continuu)
predicted_hidden_state = best_model_gaussian.predict(example_day)
predicted_weather_prob = best_model_gaussian.predict_proba(example_day)
predicted_weather_label = predicted_weather_prob.argmax(axis=1)

print(f"\nZi exemplu (prima zi - continuu):\n"
      f"Stare ascunsă prezisă: {predicted_hidden_state[0]}, "
      f"Eticheta vremii prezisă: {label_encoder_weather.inverse_transform(predicted_weather_label)[0]}")
