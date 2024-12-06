double GetAverageFromHist(const TH2D* hist) {
    double sum = 0;
    int count_bin = 0;
    for (int i = 0; i < hist->GetNbinsX(); i++) {
        if (hist->ProjectionX()->GetBinCenter(i) < 10 || hist->ProjectionX()->GetBinCenter(i) > 20) continue;
        for (int j = 0; j < hist->GetNbinsY(); j++) {
            if (hist->ProjectionY()->GetBinCenter(j) < 10 || hist->ProjectionY()->GetBinCenter(j) > 20) continue;
            sum += hist->GetBinContent(i, j);
            count_bin++;
        }
    }
    return sum / count_bin;
}

double GetStdDevFromHist(const TH2D* hist, const double mean) {
    double stddev = 0;
    int count_bin = 0;
    for (int i = 0; i < hist->GetNbinsX(); i++) {
        if (hist->ProjectionX()->GetBinCenter(i) < 10 || hist->ProjectionX()->GetBinCenter(i) > 20) continue;
        for (int j = 0; j < hist->GetNbinsY(); j++) {
            if (hist->ProjectionY()->GetBinCenter(j) < 10 || hist->ProjectionY()->GetBinCenter(j) > 20) continue;
            stddev += TMath::Power(hist->GetBinContent(i, j) - mean, 2);
            count_bin++;
        }
    }
    return TMath::Sqrt(stddev / count_bin);
}

void Interpolate() {
    auto infile_9in = TFile::Open("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/HDCInterpolate/AverageDose_Occupational_2MeV_1nC_9inHDC.root");
    auto infile_12in = TFile::Open("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/HDCInterpolate/AverageDose_Occupational_2MeV_1nC_12inHDC.root");
    auto infile_15in = TFile::Open("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/HDCInterpolate/AverageDose_Occupational_2MeV_1nC_15inHDC.root");
    auto infile_18in = TFile::Open("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/HDCInterpolate/AverageDose_Occupational_2MeV_1nC_18inHDC.root");

    auto hist_9in = (TH2D*)infile_9in->Get("PhantomWall");
    auto hist_12in = (TH2D*)infile_12in->Get("PhantomWall");
    auto hist_15in = (TH2D*)infile_15in->Get("PhantomWall");
    auto hist_18in = (TH2D*)infile_18in->Get("PhantomWall");

    double mean_9in = GetAverageFromHist(hist_9in);
    double mean_12in = GetAverageFromHist(hist_12in);
    double mean_15in = GetAverageFromHist(hist_15in);
    double mean_18in = GetAverageFromHist(hist_18in);
    double stddev_9in = GetStdDevFromHist(hist_9in, mean_9in);
    double stddev_12in = GetStdDevFromHist(hist_12in, mean_12in);
    double stddev_15in = GetStdDevFromHist(hist_15in, mean_15in);
    double stddev_18in = GetStdDevFromHist(hist_18in, mean_18in);

    std::cout << "9in: " << mean_9in << " +/- " << stddev_9in << std::endl;
    std::cout << "12in: " << mean_12in << " +/- " << stddev_12in << std::endl;
    std::cout << "15in: " << mean_15in << " +/- " << stddev_15in << std::endl;
    std::cout << "18in: " << mean_18in << " +/- " << stddev_18in << std::endl;

    auto graph = new TGraphErrors();
    graph->SetPoint(0, 9, mean_9in);
    graph->SetPointError(0, 0, stddev_9in);
    graph->SetPoint(1, 12, mean_12in);
    graph->SetPointError(1, 0, stddev_12in);
    graph->SetPoint(2, 15, mean_15in);
    graph->SetPointError(2, 0, stddev_15in);
    graph->SetPoint(3, 18, mean_18in);
    graph->SetPointError(3, 0, stddev_18in);
    auto c = new TCanvas("c", "c", 800, 800);
    c->SetLogy();
    graph->SetTitle("Average Dose Rate vs. Shielding Thickness");
    graph->GetXaxis()->SetTitle("Shielding Thickness (in)");
    graph->GetYaxis()->SetTitle("Average Dose Rate (urem/s)");
    graph->GetXaxis()->CenterTitle();
    graph->GetYaxis()->CenterTitle();
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.5);
    graph->Draw("AP");
    c->SaveAs("AverageDoseRate_vs_ShieldingThickness.png");
}