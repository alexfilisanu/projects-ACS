import matplotlib.pyplot as plt
import numpy as np

# Data for the models
models = ["Logistic Regression", "SVM", "Naive Bayes"]
accuracy = [0.9781, 0.9822, 0.8809]  # Overall accuracy
precision = [0.78, 0.78, 0.77]  # Macro avg precision
recall = [0.78, 0.81, 0.81]  # Macro avg recall
f1_score = [0.78, 0.79, 0.78]  # Macro avg F1-score

# Bar chart for precision, recall, and F1-score
metrics = ["Accuracy", "Precision", "Recall", "F1-score"]
values = np.array([accuracy, precision, recall, f1_score]).T

x = np.arange(len(models))  # Label locations
width = 0.15  # Width of bars

fig, ax = plt.subplots(figsize=(12, 6))
rects0 = ax.bar(x - 1.5*width, accuracy, width, label="Accuracy")
rects1 = ax.bar(x - 0.5*width, precision, width, label="Precision")
rects2 = ax.bar(x + 0.5*width, recall, width, label="Recall")
rects3 = ax.bar(x + 1.5*width, f1_score, width, label="F1-score")

# Add labels and title
ax.set_xlabel("Models")
ax.set_title("Model Metrics Comparison")
ax.set_xticks(x)
ax.set_xticklabels(models)
ax.set_ylim(0.5, 1.0)
ax.legend()

# Add data labels
for rect in [rects0, rects1, rects2, rects3]:
    for bar in rect:
        height = bar.get_height()
        ax.annotate(f"{height:.2f}",
                    xy=(bar.get_x() + bar.get_width() / 2, height),
                    xytext=(0, 3),
                    textcoords="offset points",
                    ha='center', va='bottom')

plt.show()
