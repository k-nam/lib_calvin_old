module IterationTest
    def IterationTest.test_01()
        array = [[1, 2], [3, 4], [5, 6]]
        array.each do |i|
            puts i[0] * i[1]
        end
    end

    def IterationTest.test_02()
        5.times { |x| puts x }
    end

    def IterationTest.test_03()
        1.upto(5) do |x|
            puts x
        end
    end
end