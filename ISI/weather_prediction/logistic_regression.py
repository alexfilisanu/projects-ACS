import pandas as pd
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder, StandardScaler

weather_df = pd.read_csv("./bucharest-weather-normalized.csv")
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
Y = weather_df['weather_encoded']

# Scaling the features
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# Splitting the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X_scaled, Y, test_size=0.2, random_state=42)

# Logistic Regression model
model = LogisticRegression(solver='lbfgs', max_iter=1000)
model.fit(X_train, y_train)

# Evaluating the model
y_pred = model.predict(X_test)
print("Accuracy:\n", accuracy_score(y_test, y_pred))
print("Classification Report:\n", classification_report(y_test, y_pred))
print("Confusion Matrix:\n", confusion_matrix(y_test, y_pred))

print("Mapping weather_encoded to original labels:")
print(dict(zip(range(len(label_encoder_weather.classes_)), label_encoder_weather.classes_)))
