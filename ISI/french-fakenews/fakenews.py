import re
import nltk
import pandas as pd
import seaborn as sns
from matplotlib import pyplot as plt
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder


nltk.download('punkt_tab')
nltk.download('stopwords')

df = pd.read_csv('./train.csv')

def preprocess_text(text):
    # Lowercase text
    text = text.lower()
    # Delete URLs
    text = re.sub('https?://\S+|www\.\S+', '', text)
    # Delete HTML tags
    text = re.sub('<.*?>', '', text)
    # Normalize spaces
    text = re.sub('\s+', ' ', text)
    # Delete punctuation, except for special French characters
    text = re.sub(r'[^\w\séàèùçâêîôûëïü]', '', text)
    # Tokenize text
    words = word_tokenize(text, language='french')
    # Delete French stopwords
    french_stopwords = set(stopwords.words('french'))
    words = [word for word in words if word not in french_stopwords]
    # Join words back into text
    return ' '.join(words)

# Apply text preprocessing on the 'text' column
df['processed_text'] = df['Text'].apply(preprocess_text)

# Fit and transform the labels to numeric values
label_encoder = LabelEncoder()
df['numeric_label'] = label_encoder.fit_transform(df['Label'])

# Show the processed DataFrame with the new numeric labels
print(df[['processed_text', 'numeric_label']].head())

# Feature (X) and Label (y)
X = df['processed_text']
y = df['numeric_label']

# Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Vectorize text data using TF-IDF
vectorizer = TfidfVectorizer(ngram_range=(1, 3), max_features=5000)
X_train = vectorizer.fit_transform(X_train)
X_test = vectorizer.transform(X_test)

# Initialize the model
model = LogisticRegression(class_weight='balanced', C=10, solver='saga', max_iter=1000)

# Train the model
model.fit(X_train, y_train)

# Predict on the test set
y_pred = model.predict(X_test)

# Evaluate the model
accuracy = accuracy_score(y_test, y_pred)
print(f"Accuracy: {accuracy:.4f}")

# Display detailed classification report
print("Classification Report:")
print(classification_report(y_test, y_pred, target_names=label_encoder.classes_))

# Confusion Matrix
cm = confusion_matrix(y_test, y_pred)

# Plotting the confusion matrix using seaborn
plt.figure(figsize=(8, 6))
sns.heatmap(cm, annot=True, fmt='d', cmap='Blues', xticklabels=label_encoder.classes_, yticklabels=label_encoder.classes_)
plt.xlabel('Predicted Label')
plt.ylabel('True Label')
plt.title('Confusion Matrix')
plt.show()
