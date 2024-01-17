from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier


def decision_tree_scikit(X, T, max_depth=3):
    X_train, X_test, T_train, T_test = train_test_split(X, T, test_size=0.2, random_state=42)

    model = DecisionTreeClassifier(random_state=42, max_depth=max_depth)
    model.fit(X_train, T_train)
    Y_train = model.predict(X_train)
    accuracy = accuracy_score(Y_train, T_train)

    Y_pred = model.predict(X_test)
    precision = precision_score(T_test, Y_pred)
    recall = recall_score(T_test, Y_pred)
    f1 = f1_score(T_test, Y_pred)

    return accuracy, precision, recall, f1
