def coin_change(total_amount, coins):
    result = {}
    
    for coin in coins:
        count = total_amount // coin
        result[coin] = count
        total_amount -= count * coin

    return result

def print_coin_change(result, total_amount):
    print(f"{total_amount} Won – ", end="")
    changes = [f"{coin} Won: {count}" for coin, count in result.items()]
    print(", ".join(changes))

# 동전 종류와 거스름돈 설정
coins = [500, 100, 50, 10]
total_amount = 2780

# 동전 교환 계산 및 결과 출력
result = coin_change(total_amount, coins)
print_coin_change(result, total_amount)
