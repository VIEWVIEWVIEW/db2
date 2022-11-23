SELECT L2.Nachname
FROM LESER as L1
    inner join Ausleihe as A
        on L1.LeserOID = A.LeserOID
    inner join Ausleihe as A2
        on A.BuchOID = A2.BuchOID
    inner join LESER as L2
        on A2.LeserOID = L2.LeserOID
WHERE  L1.LeserOID = 1 
    AND L2.LeserOID != L1.LeserOID -- Selbstreferenz rausfiltern. Wir brauchen die Bücher, die Herr Glagla ausgeliehen hat nicht.