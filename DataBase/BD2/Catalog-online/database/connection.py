import psycopg2


def connect_to_db():
    return psycopg2.connect("dbname=postgres user=postgres password=postgres host=localhost")
