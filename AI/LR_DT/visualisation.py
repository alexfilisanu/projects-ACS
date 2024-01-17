import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
from scipy import stats
from scipy.stats import chi2_contingency


def plot_distribution(data, column):
    sns.set(style="whitegrid")
    blue_color = sns.color_palette('deep')[0]
    plt.figure(figsize=(10, 6))

    if data[column].dtype == 'int' or data[column].dtype == 'float':
        sns.histplot(data[column], bins=100, kde=False, color=blue_color, edgecolor='black')
    else:
        sns.countplot(x=column, data=data)

    plt.title(f'Distribution of {column}')
    plt.xlabel(column)
    plt.ylabel('Count')
    plt.tight_layout()
    plt.show()


def plot_numeric_attributes(data, column):
    sns.set(style="whitegrid")
    plt.figure(figsize=(10, 6))
    sns.histplot(data[column], bins=10, kde=True, color='orange', edgecolor='black')
    plt.title(f'Distribution for numeric attribute {column}')
    plt.xlabel(column)
    plt.ylabel('Count')
    plt.tight_layout()
    plt.show()


def plot_categorical_attributes(data, column):
    sns.set(style="whitegrid")
    plt.figure(figsize=(10, 6))
    sns.histplot(data[column], kde=True, color='skyblue', edgecolor='black')
    plt.title(f'Distribution for categorical attribute {column}')
    plt.xlabel(column)
    plt.ylabel('Count')
    plt.tight_layout()
    plt.show()


def point_biserial_correlation(data, numeric_columns):
    correlation_results_list = []
    for column in numeric_columns:
        correlation, p_value = stats.pointbiserialr(data[column], data['Revenue'])
        correlation_results_list.append({
            'Attribute': column,
            'Point-Biserial Correlation': correlation,
            'p-value': p_value
        })
    correlation_results = pd.DataFrame(correlation_results_list)
    significant_correlations = correlation_results[correlation_results['p-value'] <= 0.05]

    print(correlation_results)

    sns.set(style="whitegrid")
    plt.figure(figsize=(12, 8))
    sns.barplot(x='Point-Biserial Correlation', y='Attribute', hue='Attribute', data=significant_correlations,
                palette='deep', legend=False)
    plt.title('Point-Biserial Correlation with "Revenue"')
    plt.xlabel('Point-Biserial Correlation Coefficient')
    plt.ylabel('Attributes')
    plt.show()


def chi_squared_test(data, categorical_columns):
    chi_squared_results_list = []

    for column in categorical_columns:
        contingency_table = pd.crosstab(data[column], data['Revenue'])
        chi2, p_value, _, _ = chi2_contingency(contingency_table)
        chi_squared_results_list.append({
            'Attribute': column,
            'Chi-squared': chi2,
            'p-value': p_value
        })
    chi_squared_results = pd.DataFrame(chi_squared_results_list)
    significant_chi_squared = chi_squared_results[chi_squared_results['p-value'] <= 0.05]

    print(chi_squared_results)

    sns.set(style="whitegrid")
    plt.figure(figsize=(12, 8))
    sns.barplot(x='Attribute', y='Chi-squared', hue='Attribute', data=significant_chi_squared, palette='Set3',
                legend=False)
    plt.title('Chi-squared Statistic for Significant Categorical Attributes')
    plt.xlabel('Attributes')
    plt.ylabel('Chi-squared Statistic')
    plt.show()
