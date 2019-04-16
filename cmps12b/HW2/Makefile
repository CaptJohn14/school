
JAVASRC    = Chessboard.java ChessPiece.java Queen.java Knight.java Rook.java King.java Pawn.java Bishop.java DoublyLinkedList.java
SOURCES    = README Makefile $(JAVASRC)
MAINCLASS  = Chessboard
CLASSES    = Chessboard.class ChessPiece.class Queen.class Knight.class Rook.class King.class Pawn.class Bishop.class DoublyLinkedList.class
JARFILE    = ChessBoard.jar

all: $(JARFILE)

$(JARFILE): $(CLASSES)
	echo Main-class: $(MAINCLASS) > Manifest
	jar cvfm $(JARFILE) Manifest $(CLASSES)
	rm Manifest

$(CLASSES): $(JAVASRC)
	javac -Xlint $(JAVASRC)

clean:
	rm $(CLASSES) $(JARFILE)
