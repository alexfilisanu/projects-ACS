import numpy as np
import pandas as pd
from sklearn.preprocessing import LabelEncoder, MinMaxScaler, StandardScaler, RobustScaler
from visualisation import plot_distribution, plot_numeric_attributes, plot_categorical_attributes, \
    point_biserial_correlation, chi_squared_test
from logistic_regression import train_logistic_full, plot_evolution, logistic_scikit
from decision_tree import decision_tree_scikit

df = pd.read_csv('dataset.csv')
# Analiza echilibrului de clase
for column in df.columns:
    plot_distribution(df, column)

# Vizualizarea atributelor numerice
numeric_columns = df.select_dtypes(include=np.number).columns
for column in numeric_columns:
    plot_numeric_attributes(df, column)

# Vizualizarea atributelor categorice
categorical_columns = df.select_dtypes(exclude=np.number).columns
for column in categorical_columns:
    plot_categorical_attributes(df, column)

# Point-Biserial Correlation
point_biserial_correlation(df, numeric_columns)

# Pearson Chi-squared test
chi_squared_test(df, categorical_columns)

# Regresie logistica - encoder pentru atributele categorice !!! doar pentru logistic regression
label_encoder = LabelEncoder()
for col in categorical_columns:
    df[col] = label_encoder.fit_transform(df[col])

X = df.drop('Revenue', axis=1)
T = df['Revenue']
scalers = [MinMaxScaler(), StandardScaler(), RobustScaler()]

# Regresie logistica - implementare manuala
for scaler in scalers:
    precision_list = []
    recall_list = []
    f1_list = []

    for _ in range(10):
        scaled_X = scaler.fit_transform(X)
        train_acc, test_acc, train_nll, test_nll, precision, recall, f1 = train_logistic_full(scaled_X, T, lr=.1,
                                                                                              epochs_no=500)

        # plot_evolution(train_acc, test_acc, train_nll, test_nll, scaler.__class__.__name__)
        precision_list.append(precision)
        recall_list.append(recall)
        f1_list.append(f1)

    print(f"\nMetrics for Scaler {scaler}:")
    print(f"Precision mean: {pd.Series(precision_list).mean():.4f}")
    print(f"Precision var: {pd.Series(precision_list).var():.4f}")
    print(f"Recall mean: {pd.Series(recall_list).mean():.4f}")
    print(f"Recall var: {pd.Series(recall_list).var():.4f}")
    print(f"F1 Score mean: {pd.Series(f1_list).mean():.4f}")
    print(f"F1 Score var: {pd.Series(f1_list).var():.4f}")

# Regresie logistica - implementare scikit-learn
#     print(f"F1 Score mean: {pd.Series(f1_list).mean():.4f}")
for scaler in scalers:
    # accuracy_list = []
    precision_list = []
    recall_list = []
    f1_list = []

    for _ in range(10):
        scaled_X = scaler.fit_transform(X)
        accuracy, precision, recall, f1 = logistic_scikit(scaled_X, T, lr=.1, epochs_no=500)

        # accuracy_list.append(accuracy)
        precision_list.append(precision)
        recall_list.append(recall)
        f1_list.append(f1)

    print(f"\nMetrics for Scaler {scaler}:")
    # print(f"Accuracy: {pd.Series(accuracy_list).mean():.4f}")
    print(f"Precision mean: {pd.Series(precision_list).mean():.4f}")
    print(f"Precision var: {pd.Series(precision_list).var():.4f}")
    print(f"Recall mean: {pd.Series(recall_list).mean():.4f}")
    print(f"Recall var: {pd.Series(recall_list).var():.4f}")
    print(f"F1 Score var: {pd.Series(f1_list).var():.4f}")

# Decision Tree - implementare scikit-learn
for scaler in scalers:
    for max_depth in range(3, 7):
        # accuracy_list = []
        precision_list = []
        recall_list = []
        f1_list = []

        for _ in range(10):
            scaled_X = scaler.fit_transform(X)
            accuracy, precision, recall, f1 = decision_tree_scikit(scaled_X, T, max_depth=max_depth)

            # accuracy_list.append(accuracy)
            precision_list.append(precision)
            recall_list.append(recall)
            f1_list.append(f1)

        print(f"\nMetrics for Max Depth {max_depth} and Scaler {scaler}:")
        # print(f"Accuracy: {pd.Series(accuracy_list).mean():.4f}")
        print(f"Precision mean: {pd.Series(precision_list).mean():.4f}")
        print(f"Precision var: {pd.Series(precision_list).var():.4f}")
        print(f"Recall mean: {pd.Series(recall_list).mean():.4f}")
        print(f"Recall var: {pd.Series(recall_list).var():.4f}")
        print(f"F1 Score mean: {pd.Series(f1_list).mean():.4f}")
        print(f"F1 Score var: {pd.Series(f1_list).var():.4f}")
