
JAVASRC    = ChessMoves.java ChessPiece.java Queen.java Knight.java Rook.java King.java Pawn.java Bishop.java DoublyLinkedList.java
SOURCES    = README Makefile $(JAVASRC)
MAINCLASS  = ChessMoves
CLASSES    = ChessMoves.class ChessPiece.class Queen.class Knight.class Rook.class King.class Pawn.class Bishop.class DoublyLinkedList.class
JARFILE    = ChessMoves.jar

all: $(JARFILE)

$(JARFILE): $(CLASSES)
	echo Main-class: $(MAINCLASS) > Manifest
	jar cvfm $(JARFILE) Manifest $(CLASSES)
	rm Manifest

$(CLASSES): $(JAVASRC)
	javac -Xlint $(JAVASRC)

clean:
	rm $(CLASSES) $(JARFILE)
