CREATE TABLE BENCH_3 (
	ID1 CHAR(50) NOT NULL,
	SP1 VARCHAR(50),
	SP2 INTEGER
);

CREATE INDEX SP2_INDEX ON BENCH_3(SP2);

-- Autoincrement
CREATE SEQUENCE SEQ_ID1;
ALTER SEQUENCE SEQ_ID1 RESTART WITH 0;

CREATE TRIGGER SEQ_ID FOR BENCH_3
BEFORE INSERT AS BEGIN
	if(new.ID1 is NULL) THEN
		new.ID1 = NEXT VALUE FOR SEQ_ID1;
END



-- Src for names: https://www.kaggle.com/datasets/kaggle/us-baby-names

-- non indexed:
select * from bench_3 where sp1 = 'Lena';

Non indexed reads: 585000
Indexed reads: 0
Execution time: 250ms


-- indexed
select * from bench_3 where sp2 = 1880;

Non indexed reads: 0
Indexed reads: 944
Execution time: 0ms


