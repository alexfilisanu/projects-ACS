INSERT INTO Materii (Nume) VALUES
    ('Matematica'),
    ('Romana'),
    ('Biologie'),
    ('Chimie'),
    ('Informatica');

INSERT INTO Clase (Nume) VALUES
    ('Clasa A'),
    ('Clasa B'),
    ('Clasa C'),
    ('Clasa D'),
    ('Clasa E');

INSERT INTO Utilizatori (Email, Parola, Tip_Utilizator) VALUES
    ('elev1@example.com', 'password_elev1', 'elev'),
    ('elev2@example.com', 'password_elev2', 'elev'),
    ('profesor1@example.com', 'password_prof1', 'profesor'),
    ('profesor2@example.com', 'password_prof2', 'profesor');

INSERT INTO Elevi (ID_Utilizator, Nume, Prenume, Clasa) VALUES
    (1, 'Popescu', 'Ion', 1),
    (2, 'Ionescu', 'Maria', 2);

INSERT INTO Profesori (ID_Utilizator, Nume, Prenume, ID_Materie) VALUES
    (3, 'Popa', 'Andrei', 3),
    (4, 'Georgescu', 'Ana', 4);

INSERT INTO Note (ID_Elev, ID_Materie, Nota) VALUES
    (1, 1, 8),
    (2, 2, 9),
    (1, 3, 7),
    (2, 4, 10);

INSERT INTO Absente (ID_Elev, ID_Materie, Motivat) VALUES
    (1, 1, true),
    (2, 2, false),
    (1, 3, false),
    (2, 4, true);
