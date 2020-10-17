




def mock(ls):
    if len(ls) == 0 or ls is None:
        return 0
    length = len(ls)
    dp = [ - float('inf') for _ in range(length)]
    dp[0] = ls[0]
    
    for i in range(1, length):
        dp[i] = max(ls[i], ls[i] + dp[i-1])

    res = -float('inf')
    for i in dp:
        res = max(res, i)
    return res

if __name__ == "__main__":
    ls = [-2, -3, 10, 11, 9, 0, 2, 1]
    ls2 = [1,3,-9,5,-2,8,100,-10,90]
    ls3 = [-1, -1, -1, -1, -1, -1, -1]
    ls4 = []
    print(mock(ls3))
    
