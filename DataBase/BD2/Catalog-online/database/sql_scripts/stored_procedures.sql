CREATE OR REPLACE PROCEDURE get_all_students_above_avg_in_subject
    (IN min_avg integer, IN subject_name varchar(50), OUT no_students integer)
AS $$
BEGIN
    SELECT COUNT(DISTINCT E.ID_elev)
    INTO no_students
    FROM Elevi E
    JOIN Note N ON E.ID_elev = N.ID_elev
    JOIN Materii M ON N.ID_materie = M.ID_materie
    WHERE E.ID_elev IN (
        SELECT E.ID_elev
        FROM Elevi E
        JOIN Note N ON E.ID_elev = N.ID_elev
        JOIN Materii M ON N.ID_materie = M.ID_materie
        WHERE M.nume = subject_name
        GROUP BY E.ID_elev
        HAVING AVG(N.nota) >= min_avg
    );
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE get_unmotivated_absences_in_class
    (IN class_name VARCHAR(20), OUT num_unmotivated_absences INTEGER)
AS $$
BEGIN
    SELECT COUNT(*)
    INTO num_unmotivated_absences
    FROM Absente A
    JOIN Elevi E ON A.ID_Elev = E.ID_Elev
    JOIN Clase C ON E.Clasa = C.ID_Clasa
    WHERE C.Nume = class_name AND A.Motivat = FALSE;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE get_motivated_absences_in_class
    (IN class_name VARCHAR(20), OUT num_unmotivated_absences INTEGER)
AS $$
BEGIN
    SELECT COUNT(*)
    INTO num_unmotivated_absences
    FROM Absente A
    JOIN Elevi E ON A.ID_Elev = E.ID_Elev
    JOIN Clase C ON E.Clasa = C.ID_Clasa
    WHERE C.Nume = class_name AND A.Motivat = TRUE;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE get_subjects_above_avg_in_class
    (IN class_name VARCHAR(50), IN min_avg INTEGER, OUT no_subjects INTEGER)
AS $$
BEGIN
    SELECT COUNT(DISTINCT M.ID_materie)
    INTO no_subjects
    FROM Materii M
    JOIN Note N ON M.ID_materie = N.ID_materie
    JOIN Elevi E ON N.ID_elev = E.ID_elev
    JOIN Clase C ON E.Clasa = C.ID_Clasa
    WHERE C.Nume = class_name
    GROUP BY M.ID_materie
    HAVING AVG(N.nota) >= min_avg;
END;
$$ LANGUAGE plpgsql;
