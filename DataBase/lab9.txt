-- numarul maxim de angajati in functie de departament

SELECT MAX(COUNT(*))
    FROM EMP A 
    GROUP BY A.DEPTNO;

-- numele departamentului cu cei mai multi angajati

SELECT D.DNAME, COUNT(*)
    FROM EMP C JOIN DEPT D ON C.DEPTNO = D.DEPTNO
    GROUP BY D.DNAME
    HAVING COUNT(*) = (SELECT MAX(COUNT(*))
                        FROM EMP A 
                        GROUP BY A.DEPTNO);

-- sa se determine gradul salarial si numarul de angajati care fac parte din el
-- pentru gradul cu cei mai multi angajati  

SELECT S.GRADE, COUNT(*)
    FROM EMP C JOIN SALGRADE S ON C.SAL BETWEEN S.LOSAL AND S.HISAL
    GROUP BY S.GRADE
    HAVING COUNT(*) = (SELECT MAX(COUNT(*))
                        FROM EMP A JOIN SALGRADE S ON A.SAL BETWEEN S.LOSAL AND S.HISAL
                        GROUP BY S.GRADE);

-- sa se afiseze care este departamentul in care sunt angajatii cu cele mai multe 
-- functii de acelasi fel, afisand departamentul, functia si nr de angajati cu acea functie

SELECT A.DNAME, B.JOB, COUNT(*)
FROM EMP B JOIN DEPT A ON B.DEPTNO = A.DEPTNO
GROUP BY A.DNAME, B.JOB
HAVING COUNT(*) = (SELECT MAX(COUNT(*))
                    FROM EMP B
                    GROUP BY DEPTNO, JOB);

-- sa se afiseze care este anul in care au venit cei mai multi angajati in departamentul sales

SELECT EXTRACT(YEAR FROM A.HIREDATE), COUNT(*)
    FROM EMP A JOIN DEPT B ON A.DEPTNO = B.DEPTNO
    WHERE B.DNAME = 'SALES'
    GROUP BY EXTRACT(YEAR FROM A.HIREDATE)
    HAVING COUNT(*) = (SELECT MAX(COUNT(*))
                        FROM EMP A JOIN DEPT B ON A.DEPTNO = B.DEPTNO
                        WHERE B.DNAME = 'SALES'
                        GROUP BY EXTRACT(YEAR FROM A.HIREDATE));

-- sa se selecteze toti angajatii care au venit in firma dupa cel mai bine platit angajat
-- din sales afisand numele angajatului, data angajarii si sal mediu din firma

SELECT A.ENAME, A.HIREDATE, (SELECT AVG(SAL) FROM EMP) SALMED
    FROM EMP A 
    WHERE A.HIREDATE > (SELECT B.HIREDATE
                        FROM EMP B JOIN DEPT C ON B.DEPTNO = C.DEPTNO
                        WHERE B.SAL + NVL(B.COMM, 0) = (SELECT MAX(D.SAL + NVL(D.COMM, 0))
                                                        FROM EMP D JOIN DEPT E ON D.DEPTNO = E.DEPTNO
                                                        WHERE E.DNAME = 'SALES')
                        AND C.DNAME = 'SALES');

-- sa se selecteze pentru fiecare angajat care castiga mai putin decat seful sau
-- numele sau, numele sefului si sal mediu din departamentul sau
-- 2 metode: subcerere corelata pe SELECT

SELECT A.ENAME, B.ENAME, (SELECT AVG(SAL) FROM EMP C WHERE C.DEPTNO = A.DEPTNO) SALMED
    FROM EMP A JOIN EMP B ON A.MGR = B.EMPNO
    WHERE A.SAL + NVL(A.COMM, 0) < B.SAL + NVL(B.COMM, 0);

-- aceasei cerinta cu sucerere necorlata pe FROM

SELECT A.ENAME, B.ENAME, C.SALMED
    FROM EMP A JOIN EMP B ON A.MGR = B.EMPNO
    JOIN (SELECT D.DEPTNO, AVG(D.SAL) SALMED
            FROM EMP D
            GROUP BY D.DEPTNO) C ON A.DEPTNO = C.DEPTNO
    WHERE A.SAL + NVL(A.COMM, 0) < B.SAL + NVL(B.COMM, 0);

-- sa se selecteze pentru fiecare angajat al carui nume nu se termina cu es si are acelasi
-- grad salarial cu Scott, numele, salariul, gradul salarial si salariul max din firma

SELECT A.ENAME, B.GRADE, (SELECT MAX(SAL) FROM EMP) MAXS
    FROM EMP A JOIN SALGRADE B ON A.SAL BETWEEN B.LOSAL AND B.HISAL
    WHERE B.GRADE = (SELECT C.GRADE
                    FROM EMP D JOIN SALGRADE C ON D.SAL BETWEEN C.LOSAL AND C.HISAL
                    WHERE D.ENAME = 'SCOTT')
    AND A.ENAME NOT LIKE '%ES';

-- departamentele cu cei mai multi angajati in ordine descrescatoare(subcerere pe ORDER BY)

SELECT A.ENAME, B.DNAME
    FROM EMP A JOIN DEPT B ON A.DEPTNO = B.DEPTNO
    ORDER BY (SELECT COUNT(*)
                FROM EMP C 
                WHERE C.DEPTNO = A.DEPTNO) DESC;

-- afisati toti angajatii impreuna cu salariile lor in ordinea crescatoare
-- care au salariu mai mare decat el
-- mai intai angajatul cu cei mai putini angajati cu salariu > decat el

SELECT A.ENAME, A.SAL
    FROM EMP A
    ORDER BY (SELECT COUNT(*)
                FROM EMP B
                WHERE B.SAL < A.SAL);

SELECT A.ENAME, A.SAL
    FROM EMP A
    ORDER BY (A.SAL);

-- afiseaza angajati daca sunt si ei sefi

SELECT A.ENAME
    FROM EMP A
    WHERE EXISTS (SELECT B.ENAME
                    FROM EMP B
                    WHERE B.MGR = A.EMPNO);

-- sa se selecteze toate departamentele care nu au angajati in gradele 4 si 5

SELECT A.DNAME
    FROM DEPT A
    WHERE NOT EXISTS (SELECT B.DEPTNO
                        FROM EMP B JOIN SALGRADE C ON B.SAL BETWEEN C.LOSAL AND C.HISAL
                        WHERE B.DEPTNO = A.DEPTNO
                        AND C.GRADE IN (4, 5));
