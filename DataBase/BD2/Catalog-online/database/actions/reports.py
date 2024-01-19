def get_report_data(cursor, procedure_name, *params):
    cursor.execute(f'CALL {procedure_name}({", ".join(["%s" for _ in params])})', params)
    return cursor.fetchall()


def get_total_students(cursor):
    cursor.execute('SELECT COUNT(*) FROM elevi')
    return cursor.fetchone()[0]


def get_total_subjects(cursor):
    cursor.execute('SELECT COUNT(*) FROM materii')
    return cursor.fetchone()[0]


def get_student_grades(cursor, email):
    cursor.execute('''
                    SELECT Data, Nota, Materii.Nume 
                    FROM Note 
                    JOIN Materii ON Note.ID_Materie = Materii.ID_Materie 
                    WHERE ID_Elev = (SELECT ID_Elev 
                                     FROM Elevi 
                                     JOIN Utilizatori ON Elevi.ID_Utilizator = Utilizatori.ID_Utilizator 
                                     WHERE email = %s)
                   ''', (email,))
    return cursor.fetchall()


def get_student_absences(cursor, email):
    cursor.execute('''
                    SELECT Data, Materii.Nume, Motivat 
                    FROM Absente 
                    JOIN Materii ON Absente.ID_Materie = Materii.ID_Materie 
                    WHERE ID_Elev = (SELECT ID_Elev 
                                     FROM Elevi 
                                     JOIN Utilizatori ON Elevi.ID_Utilizator = Utilizatori.ID_Utilizator 
                                     WHERE Email = %s)
                ''', (email,))
    return cursor.fetchall()
