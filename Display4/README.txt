 
GPIO Pin Mapping (moddfied from "GPIO readall" ouput)
******************************************************
 
 +-----+-----+---------+------+---+---Pi 3---+---+------+---------+-----+-----+
 | Dis | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | Dis |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |     |   8 |   SDA.1 |  OUT | 1 |  3 || 4  |   |      | 5v      |     |     |
 |     |   9 |   SCL.1 |   IN | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   1 |   7 | GPIO. 7 |  OUT | 1 |  7 || 8  | 0 | IN   | TxD     | 15  |     |
 |     |     |      0v |      |   |  9 || 10 | 1 | IN   | RxD     | 16  |     |
 |   2 |   0 | GPIO. 0 |  OUT | 1 | 11 || 12 | 1 | OUT  | GPIO. 1 | 1   | 5   |
 |   3 |   2 | GPIO. 2 |  OUT | 1 | 13 || 14 |   |      | 0v      |     |     |
 |   4 |   3 | GPIO. 3 |  OUT | 1 | 15 || 16 | 1 | OUT  | GPIO. 4 | 4   | 7   |
 |     |     |    3.3v |      |   | 17 || 18 | 1 | OUT  | GPIO. 5 | 5   | 10  |
 |     |  12 |    MOSI | ALT0 | 0 | 19 || 20 |   |      | 0v      |     |     |
 |     |  13 |    MISO |   IN | 0 | 21 || 22 | 1 | OUT  | GPIO. 6 | 6   | 11  |
 |     |  14 |    SCLK | ALT0 | 0 | 23 || 24 | 1 | IN   | CE0     | 10  |     |
 |     |     |      0v |      |   | 25 || 26 | 1 | IN   | CE1     | 11  |     |
 |     |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 1 | IN   | SCL.0   | 31  |     |
 |     |  21 | GPIO.21 |  OUT | 0 | 29 || 30 |   |      | 0v      |     |     |
 |     |  22 | GPIO.22 |  OUT | 0 | 31 || 32 | 1 | OUT  | GPIO.26 | 26  | 6   |
 |     |  23 | GPIO.23 |  OUT | 0 | 33 || 34 |   |      | 0v      |     |     |
 |     |  24 | GPIO.24 |  OUT | 0 | 35 || 36 | 1 | OUT  | GPIO.27 | 27  | 8   |
 |     |  25 | GPIO.25 |  OUT | 0 | 37 || 38 | 1 | OUT  | GPIO.28 | 28  | 9   |
 |     |     |      0v |      |   | 39 || 40 | 1 | OUT  | GPIO.29 | 29  | 12  |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+---Pi 3---+---+------+---------+-----+-----+

Display	
  Pin		GPIO		Segment
=======		=======		=========
	1		GPIO 7		Left-Bottom	
	2		GPIO 0		Bottom
	3		GPIO 2		Decimal
	4		GPIO 3		Right-Bottom
	5		GPIO 1		Middle
	6		GPIO 26		#Fourth
	7		GPIO 4		Right-Top
	8		GPIO 27		#Third
	9		GPIO 28		#Second
	10		GPIO 5		Left-Top
	11		GPIO 6		Top
	12		GPIO 29		#First
