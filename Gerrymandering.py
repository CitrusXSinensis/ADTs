import sys

res = False
args, A = [], [0]
for line in sys.stdin: args.append(line)
n = int(args[0].split()[0])
m = int(args[0].split()[1])
for num in args[1].split(): A.append(int(num))
a = sum(A)
    
dp = [[[0 for _ in range(m*n+1)] for _ in range(n+1)] for _ in range(n+1)]

# base case
for i in range(n+1):
    dp[i][0][0] = 1
dp[1][1][A[1]] = 1

# recursive build up dp
for i in range(2, n+1):
    for j in range(max(i + 1, n//2 + 1)):
        for l in range(n*m+1):
            if dp[i-1][j][l] or dp[i-1][j-1][l-A[i]]:
                dp[i][j][l] = 1

curl = -1

for ind, item in enumerate(dp[-1][n//2]):
    if ind > m*n/4 and ind < a - m*n/4:
        if item == 1:
            curl = ind
            res = True
            break

if not res:
    print('false')

# backtrack
else:
    print('true')
    stack = []
    curi, curj = n, n//2
    while curi > 0:
        if dp[curi-1][curj][curl] == 1:
            curi -= 1
        elif dp[curi-1][curj-1][curl-A[curi]]:
            curl -= A[curi]
            stack.append(str(curi))
            curi -= 1
            curj -= 1
        else: break
    print(' '.join(stack))
        








