/*
    Sa se selecteze toti angajatiii care castiga mai mult decat sefii lor si care au venit in firma
    in anul in care s au angajat cei mai multi angajati.

    Afisare: Nume_angajat, Denumire_departament, Grad_salarial, Venit, Venit_sef, Nume_sef
*/

/* anul in care s au angajat cei mai multi angajati */
SELECT EXTRACT(YEAR FROM A.HIREDATE) AN, COUNT(*)
    FROM EMP A
    GROUP BY EXTRACT(YEAR FROM A.HIREDATE)
    HAVING COUNT(*) = (SELECT MAX(COUNT(*))
                        FROM EMP A
                        GROUP BY EXTRACT(YEAR FROM A.HIREDATE));

SELECT A.ENAME Nume_angajat, B.DNAME Denumire_departament, S.GRADE Grad_salarial, A.SAL + NVL(A.COMM, 0) Venit, 
        SEF.SAL + NVL(SEF.COMM, 0) Venit_sef, SEF.ENAME Nume_sef
    FROM EMP A, EMP SEF, DEPT B, SALGRADE S
    WHERE A.MGR = SEF.EMPNO
        AND A.SAL BETWEEN S.LOSAL AND S.HISAL
        AND A.DEPTNO = B.DEPTNO
        AND A.SAL + NVL(A.COMM, 0) > SEF.SAL + NVL(SEF.COMM, 0)
        AND EXTRACT(YEAR FROM A.HIREDATE) = (SELECT EXTRACT(YEAR FROM A.HIREDATE) AN
                                                FROM EMP A
                                                GROUP BY EXTRACT(YEAR FROM A.HIREDATE)
                                                HAVING COUNT(*) = (SELECT MAX(COUNT(*))
                                                                    FROM EMP A
                                                                    GROUP BY EXTRACT(YEAR FROM A.HIREDATE)));
