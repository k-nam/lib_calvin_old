module test_iteeration
    def test_01()
        array = [[1, 2], [3, 4], [5, 6]]
        array.each do |i|
            puts i[0] * i[1]
        end
    end
end