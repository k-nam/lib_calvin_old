package status;

public class Score {
	public int lightScore;
	public int tenScore;
	public int fiveScore;
	public int shellScore;
	
	public int godoriScore;
	public int hongDanScore;
	public int chungDanScore;
	public int choDanScore;
	
	public Score(int lightScore, int tenScore, int fiveScore, int shellScore,
			int godoriScore, int hongDanScore, int chungDanScore, int choDanScore) {
		this.lightScore = lightScore;
		this.tenScore = tenScore;
		this.fiveScore = fiveScore;
		this.shellScore = shellScore;
		this.godoriScore = godoriScore;
		this.hongDanScore = hongDanScore;
		this.chungDanScore = chungDanScore;
		this.choDanScore = choDanScore;
	}
	
	public int getTotalScore() {
		return lightScore + tenScore + fiveScore + shellScore + 
							godoriScore + hongDanScore + chungDanScore + choDanScore;				
	}
}
