module ContainerTest
    def ContainerTest.test_01()
        empty1 = []
        empty2 = Array.new
        words1 = ['Calvin', 'Tom']
        words2 = %w{ Calvin Tom}
        puts words1, words2
    end

    def ContainerTest.test_02()
        dictionary = { 'apple' => 'リンゴ' }
        data = Hash.new(0)
        data['height'] = 7
        puts dictionary['wood'], data['width']
    end
end