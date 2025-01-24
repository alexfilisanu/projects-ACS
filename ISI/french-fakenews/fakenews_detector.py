import re
import nltk
import pandas as pd
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import LabelEncoder

nltk.download('punkt_tab')
nltk.download('stopwords')

train_df = pd.read_csv('./train.csv')
test_df = pd.read_csv('./test.csv')

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
train_df['processed_text'] = train_df['Text'].apply(preprocess_text)
test_df['processed_text'] = test_df['Text'].apply(preprocess_text)

# Fit and transform the labels to numeric values
label_encoder = LabelEncoder()
train_df['numeric_label'] = label_encoder.fit_transform(train_df['Label'])

# Feature (X) and Label (y)
X_train = train_df['processed_text']
y_train = train_df['numeric_label']
X_test = test_df['processed_text']

# Vectorize text data using TF-IDF
vectorizer = TfidfVectorizer(ngram_range=(1, 3), max_features=5000)
X_train = vectorizer.fit_transform(X_train)
X_test = vectorizer.transform(X_test)

# Initialize the model
model = LogisticRegression(class_weight='balanced', C=10, solver='saga', max_iter=1000)

# Train the model
model.fit(X_train, y_train)

# Save the predictions to a CSV file
test_df['Label'] = label_encoder.inverse_transform(model.predict(X_test))
test_df.drop('processed_text', axis=1, inplace=True)
test_df.to_csv('test_completed.csv', index=False)
