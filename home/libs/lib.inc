PrintWord: PROCEDURE(n);
	DECLARE n WORD;
	DECLARE (i, count) BYTE, digits(20) BYTE;

	IF n = 0 THEN DO;
		CALL dos_std_out('0');
		RETURN;
	END;

	count = 0;
	DO WHILE n > 0;
		digits(LAST(digits)-count) = (n MOD 10) + '0';
		n = n / 10;
		count = count + 1;
	END;

	DO i = LAST(digits)-count+1 TO LAST(digits);
		CALL dos_std_out(digits(i));
	END;
END PrintWord;

Println: PROCEDURE;
	CALL dos_std_string_out(@(13, 10, '$'));
END Println;

ReadChar: PROCEDURE BYTE;
	DECLARE buf STRUCTURE (req BYTE, read BYTE, chars(1) BYTE, cr BYTE);

	buf.req = 2;
	CALL dos_buffered_std_in(@buf); CALL Println;

	RETURN buf.chars(0);
END ReadChar;

ReadWord: PROCEDURE WORD;
	DECLARE (sign, n) WORD, (i, c) BYTE;
	DECLARE buf STRUCTURE (req BYTE, read BYTE, chars(20) BYTE, cr BYTE);

	buf.req = 20;
	CALL dos_buffered_std_in(@buf); CALL Println;

	sign = 1;
	n = 0;
	DO i = 0 TO buf.read;
		c = buf.chars(i);
		IF (i = 0) AND (c = '-') THEN sign = -1;
		IF (c >= '0') AND (c <= '9') THEN n = n * 10 + c - '0';
	END;

	RETURN sign * n;
END ReadWord;

DECLARE _seed WORD INITIAL(1);
Srand: PROCEDURE(seed);
	DECLARE seed WORD;
	_seed = seed;
END Srand;

Rand: PROCEDURE WORD;
	DECLARE a WORD DATA (16807), c WORD DATA (0), m WORD DATA (0FFFFh);
	_seed = (_seed * a + c) MOD m;
	RETURN _seed;
END Rand;
