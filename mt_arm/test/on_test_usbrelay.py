import usbrelay_py
import time
# Note that a call to count() is required to enumerate the attached relays
# before attempting to operate the relays

count = usbrelay_py.board_count()
print("Count: ",count)

boards = usbrelay_py.board_details()
print("Boards: ",boards)

for board in boards:
    print("Board: ",board)
    relay = 1
    while(relay < board[1]+1):
        result = usbrelay_py.board_control(board[0],relay,1)
        print("Result: ",result)
        relay += 1