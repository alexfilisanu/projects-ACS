CREATE TABLE Materii (
    ID_Materie SERIAL PRIMARY KEY,
    Nume VARCHAR(50)
);

CREATE TABLE Clase (
    ID_Clasa SERIAL PRIMARY KEY,
    Nume VARCHAR(20)
);

CREATE TABLE Utilizatori (
    ID_Utilizator SERIAL PRIMARY KEY,
    Email VARCHAR(50),
    Parola VARCHAR(255),
    Tip_Utilizator VARCHAR(20)
);

CREATE TABLE Elevi (
    ID_Elev SERIAL PRIMARY KEY,
    ID_Utilizator INTEGER REFERENCES Utilizatori(ID_Utilizator),
    Nume VARCHAR(50),
    Prenume VARCHAR(50),
    Clasa INTEGER REFERENCES Clase(ID_Clasa)
);

CREATE TABLE Profesori (
    ID_Profesor SERIAL PRIMARY KEY,
    ID_Utilizator INTEGER REFERENCES Utilizatori(ID_Utilizator),
    Nume VARCHAR(50),
    Prenume VARCHAR(50),
    ID_Materie INTEGER REFERENCES Materii(ID_Materie)
);

CREATE TABLE Note (
    ID_Nota SERIAL PRIMARY KEY,
    ID_Elev INTEGER REFERENCES Elevi(ID_Elev),
    ID_Materie INTEGER REFERENCES Materii(ID_Materie),
    Nota INTEGER,
    Data DATE DEFAULT CURRENT_DATE
);

CREATE TABLE Absente (
    ID_Absenta SERIAL PRIMARY KEY,
    ID_Elev INTEGER REFERENCES Elevi(ID_Elev),
    ID_Materie INTEGER REFERENCES Materii(ID_Materie),
    Data DATE DEFAULT CURRENT_DATE,
    Motivat BOOLEAN
);
