import numpy as np
import seaborn as sns
from matplotlib import pyplot as plt
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score
from sklearn.model_selection import train_test_split


def logistic(x):
    np.seterr(over='ignore')

    try:
        result = 1 / (1 + np.exp(-x))
    except OverflowError or RuntimeWarning:
        result = 1
    np.seterr(over='warn')

    return result


def nll(Y, T):
    result = 0
    for y, t in zip(Y, T):
        if y != 0:
            result -= t * np.log(y)
        if y != 1:
            result -= (1 - t) * np.log(1 - y)

    return result


def accuracy(Y, T):
    correct_predictions = np.sum(np.round(Y) == T)
    total_examples = len(T)

    return correct_predictions / total_examples


def train_logistic(X, T, lr=.01, epochs_no=100):
    (N, D) = X.shape
    X_hat = np.concatenate([X, np.ones((N, 1))], axis=1)
    W = np.random.randn(D + 1)

    for epoch in range(epochs_no):
        Y = logistic(np.dot(X_hat, W))
        gradient = np.dot(X_hat.T, Y - T) / N
        W -= lr * gradient

    return W


def predict_logistic(X, W):
    (N, D) = X.shape
    X_hat = np.concatenate([X, np.ones((N, 1))], axis=1)
    Y = logistic(np.dot(X_hat, W))
    return Y


def train_logistic_full(X, T, lr=.01, epochs_no=100):
    np.seterr(over='ignore')

    X_train, X_test, T_train, T_test = train_test_split(X, T, test_size=0.2, random_state=42)

    (N_train, D) = X_train.shape
    (N_test, _) = X_test.shape

    X_train1 = np.concatenate([np.ones((N_train, 1)), X_train], axis=1)
    X_test1 = np.concatenate([np.ones((N_test, 1)), X_test], axis=1)

    W = np.random.randn(D + 1)
    train_acc, test_acc = [], []
    train_nll, test_nll = [], []
    W_trace = [W.copy()]

    for epoch in range(epochs_no):
        try:
            Y_train = 1. / (1. + np.exp(-np.dot(X_train1, W)))
        except OverflowError or RuntimeWarning:
            Y_train = 1

        gradient = np.dot(X_train1.T, Y_train - T_train) / N_train
        W -= lr * gradient

        try:
            Y_test = 1. / (1. + np.exp(-np.dot(X_test1, W)))
        except OverflowError or RuntimeWarning:
            Y_test = 1

        train_acc.append(accuracy(Y_train, T_train))
        test_acc.append(accuracy(Y_test, T_test))
        train_nll.append(nll(Y_train, T_train) / T_train.__len__())
        test_nll.append(nll(Y_test, T_test) / T_test.__len__())
        W_trace.append(W.copy())

    np.seterr(over='warn')

    Y_pred = predict_logistic(X_test, W)
    Y_pred_binary = np.round(Y_pred)
    precision = precision_score(T_test, Y_pred_binary)
    recall = recall_score(T_test, Y_pred_binary)
    f1 = f1_score(T_test, Y_pred_binary)

    return train_acc, test_acc, train_nll, test_nll, precision, recall, f1


def plot_evolution(train_acc, test_acc, train_nll, test_nll, scaler_name):
    epochs_no = len(train_acc)
    fig, (ax1, ax2) = plt.subplots(2, 1)
    ax1.plot(range(epochs_no), train_acc, sns.xkcd_rgb["green"], label="Train Accuracy")
    ax1.plot(range(epochs_no), test_acc, sns.xkcd_rgb["red"], label="Test Accuracy")
    ax1.set_ylabel("Accuracy")
    ax1.legend(loc='lower right', ncol=1)

    ax2.plot(range(epochs_no), train_nll, sns.xkcd_rgb["green"], label="Train NLL")
    ax2.plot(range(epochs_no), test_nll, sns.xkcd_rgb["red"], label="Test NLL")
    ax2.set_xlabel("iteration")
    ax2.set_ylabel("NLL")
    ax2.legend(loc='upper right', ncol=1)

    plt.suptitle(f'Evolution of accuracy and NLL for {scaler_name}')
    plt.show()


def logistic_scikit(X, T, lr=.01, epochs_no=100):
    X_train, X_test, T_train, T_test = train_test_split(X, T, test_size=0.2, random_state=42)

    model = LogisticRegression(random_state=42, solver='liblinear', max_iter=epochs_no, tol=lr)
    model.fit(X_train, T_train)
    Y_train = model.predict(X_train)
    accuracy = accuracy_score(Y_train, T_train)

    Y_pred = model.predict(X_test)
    precision = precision_score(T_test, Y_pred)
    recall = recall_score(T_test, Y_pred)
    f1 = f1_score(T_test, Y_pred)

    return accuracy, precision, recall, f1
