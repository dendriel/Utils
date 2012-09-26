require('class')

Calc = class()

function Calc:init()
end

function Calc:sum(a, b)
    print(a+b)
end

function Calc:sub(a, b)
    print(b - a)
end

c = Calc()
c:sum(10,5)
c:sub(10,5)
