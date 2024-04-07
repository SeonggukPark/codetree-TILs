# 강도 구하기
def intensity(c):
    res = 0
    for i in range(n//2):
        for j in range(i+1, n//2):
            res += p[c[i]][c[j]]
            res += p[c[j]][c[i]]

    return res

# 최솟값 구하기
def dfs(idx, depth):
    global ans

    if depth == n//2:
        tmp2 = [i for i in range(n) if i not in tmp]
        ans = min(ans, abs(intensity(tmp) - intensity(tmp2)))

    for i in range(idx+1, n):
        tmp.append(i)
        dfs(i, depth+1)
        tmp.pop()


if __name__ == '__main__':
    n = int(input())

    # 상성 리스트 저장
    p = []
    for _ in range(n):
        p.append(list(map(int, input().split())))

    tmp = [0]
    ans = float("INF")
    dfs(0, 1)
    print(ans)