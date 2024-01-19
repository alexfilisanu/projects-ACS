from database.connection import connect_to_db


def authenticate_user(email, password):
    conn = connect_to_db()
    cursor = conn.cursor()

    try:
        cursor.execute("SELECT Tip_Utilizator FROM Utilizatori WHERE Email = %s AND Parola = %s", (email, password))
        result = cursor.fetchone()

        if result:
            return result[0]

        return None

    finally:
        if cursor:
            cursor.close()
        if conn:
            conn.close()