from flask import Flask, render_template, request, redirect, url_for, session
from database.connection import connect_to_db
from database.actions.reports import get_report_data, get_total_students, get_total_subjects, get_student_grades, \
    get_student_absences
from database.auth import authenticate_user

app = Flask(__name__)
app.secret_key = 'strong_secret_key'


@app.route('/', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        email = request.form['email']
        password = request.form['password']

        user_role = authenticate_user(email, password)

        if user_role:
            session['username'] = email
            session['role'] = user_role
            return redirect(url_for('dashboard'))

    return render_template('login.html')


@app.route('/dashboard')
def dashboard():
    if 'username' in session:
        role = session.get('role')

        if role == 'elev':
            return render_template('elev_dashboard.html')
        elif role == 'profesor':
            return render_template('profesor_dashboard.html')

    return redirect(url_for('login'))


@app.route('/logout')
def logout():
    session.pop('username', None)
    return redirect(url_for('login'))


@app.route('/students_above_avg_in_subject', methods=['GET', 'POST'])
def students_above_avg_in_subject():
    conn = None
    cursor = None

    try:
        conn = connect_to_db()
        cursor = conn.cursor()

        if request.method == 'POST':
            subject = request.form['subject']
            grade = int(request.form['grade'])

            report_data = get_report_data(cursor, 'get_all_students_above_avg_in_subject',
                                          grade, subject, 0)
            total_students = get_total_students(cursor)
            report_data_strings = [", ".join(map(str, row)) for row in report_data]

            return render_template('students_above_avg_in_subject.html', reports=report_data_strings,
                                   total_students=total_students)

        return render_template('students_above_avg_in_subject.html', reports=None,
                               total_students=None)

    finally:
        if cursor:
            cursor.close()
        if conn:
            conn.close()


@app.route('/get_grades', methods=['POST'])
def get_grades():
    conn = None
    cursor = None

    try:
        conn = connect_to_db()
        cursor = conn.cursor()

        if 'username' in session and session['role'] == 'elev':
            student_username = session['username']
            grades_data = get_student_grades(cursor, student_username)
            return render_template('grades_template.html', grades=grades_data)
        else:
            return redirect(url_for('dashboard'))

    finally:
        if cursor:
            cursor.close()
        if conn:
            conn.close()


@app.route('/get_absences', methods=['POST'])
def get_absences():
    conn = None
    cursor = None

    try:
        conn = connect_to_db()
        cursor = conn.cursor()
        if 'username' in session and session['role'] == 'elev':
            student_username = session['username']
            absences_data = get_student_absences(cursor, student_username)
            return render_template('absences_template.html', absences=absences_data)
        else:
            return redirect(url_for('login'))

    finally:
        if cursor:
            cursor.close()
        if conn:
            conn.close()


@app.route('/absences_in_class', methods=['GET', 'POST'])
def absences_in_class():
    conn = None
    cursor = None

    try:
        conn = connect_to_db()
        cursor = conn.cursor()

        if request.method == 'POST':
            className = request.form['className']
            unmotivated_raport = get_report_data(cursor, 'get_unmotivated_absences_in_class',
                                                 className, 0)
            motivated_raport = get_report_data(cursor, 'get_motivated_absences_in_class',
                                               className, 0)
            unmotivated_strings = [", ".join(map(str, row)) for row in unmotivated_raport]
            motivated_strings = [", ".join(map(str, row)) for row in motivated_raport]

            return render_template('absences_in_class.html', num_unmotivated_absences=unmotivated_strings,
                                   num_motivated_absences=motivated_strings)

        return render_template('absences_in_class.html', num_unmotivated_absences=None,
                               num_motivated_absences=None)

    finally:
        if cursor:
            cursor.close()
        if conn:
            conn.close()


@app.route('/subjects_above_avg_in_class', methods=['GET', 'POST'])
def subjects_above_avg_in_class():
    conn = None
    cursor = None

    try:
        conn = connect_to_db()
        cursor = conn.cursor()

        if request.method == 'POST':
            className = request.form['classNameMin']
            grade = int(request.form['gradeMin'])

            report_data = get_report_data(cursor, 'get_subjects_above_avg_in_class',
                                          className, grade, 0)
            total_subjects = get_total_subjects(cursor)
            report_data_strings = [", ".join(map(str, row)) for row in report_data]

            return render_template('subjects_above_avg_in_class.html', reports=report_data_strings,
                                   total_subjects=total_subjects)

        return render_template('subjects_above_avg_in_class.html', reports=None,
                               total_subjects=None)

    finally:
        if cursor:
            cursor.close()
        if conn:
            conn.close()


if __name__ == '__main__':
    app.run(debug=True)
