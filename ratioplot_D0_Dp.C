void openInputFiles(const char* particle_type1, const char* particle_type2, TFile*& file1, TFile*& file2, TTree*& tree1, TTree*& tree2) {
    TString file_path1 = Form("%s/Gen%sMC_output.root", particle_type1, particle_type1);
    TString file_path2 = Form("%s/Gen%sMC_output.root", particle_type2, particle_type2);

    file1 = new TFile(file_path1, "READ");
    file2 = new TFile(file_path2, "READ");

    if (!file1 || file1->IsZombie() || !file2 || file2->IsZombie()) {
        std::cerr << "Error: Could not open input files" << std::endl;
        delete file1; 
        delete file2;
        file1 = nullptr;
        file2 = nullptr;
        return;
    }

    TString tree_name = "MCDecayTree";
    tree1 = (TTree*)file1->Get(tree_name);
    tree2 = (TTree*)file2->Get(tree_name);

    if (!tree1 || !tree2) {
        std::cerr << "Error: Could not find TTrees in input files" << std::endl;
        file1->Close();
        file2->Close();
        delete file1;
        delete file2;
        file1 = nullptr;
        file2 = nullptr;
        return;
    }

    std::cout << "Input files read successfully." << std::endl;
    std::cout << "Tree " << tree_name << " found in both files." << std::endl;
}

void ratioplot_D0_Dp() {
    TCanvas *c1 = new TCanvas();

    TFile* file1 = nullptr;
    TFile* file2 = nullptr;
    TTree* tree1 = nullptr;
    TTree* tree2 = nullptr;
    const char* particle_type1 = "D0";
    const char* particle_type2 = "Dp";
    openInputFiles(particle_type1, particle_type2, file1, file2, tree1, tree2);

    if (!file1 || !file2 || !tree1 || !tree2) {
        std::cerr << "Error: Failed to open input files or trees." << std::endl;
        delete file1; 
        delete file2;
        delete tree1;
        delete tree2;
        return;
    }

    std::cout << "Starting processing." << std::endl;

    int num_rapidity_bins = 6;
    double rapidity_min = 2.0;
    double rapidity_max = 5.0;
    double rapidity_step = (rapidity_max - rapidity_min) / num_rapidity_bins;
    double rapidity_bins[num_rapidity_bins + 1];
    for (int i = 0; i <= num_rapidity_bins; ++i) {
        rapidity_bins[i] = rapidity_min + i * rapidity_step;
    }

    int num_pt_bins = 12;
    double pt_bins[] = {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 12000, 14000};

    std::cout << "Calculating the ratio plots..." << std::endl;

    std::vector<TRatioPlot*> ratio_plots;

    for (int i = 0; i < num_rapidity_bins; ++i) {

        TCut cuts1 = Form("%s_TRUEY >= %f && %s_TRUEY < %f && abs(Km_MC_MOTHER_ID) == 421 && abs(Hp_MC_MOTHER_ID) == 421", particle_type1, rapidity_bins[i], particle_type1, rapidity_bins[i+1]);
        TCut cuts2 = Form("%s_TRUEY >= %f && %s_TRUEY < %f && abs(Km_MC_MOTHER_ID) == 411 && abs(H1_MC_MOTHER_ID) == 411 && abs(H2_MC_MOTHER_ID) == 411", particle_type2, rapidity_bins[i], particle_type2, rapidity_bins[i+1]);

        std::cout << "    Cuts were made for iteration " << i << " out of " << num_rapidity_bins << "." << std::endl;

        TH1D* hist1 = new TH1D(Form("hist1_%d", i), Form("%s_TRUEPT", particle_type1), num_pt_bins, pt_bins);
        TH1D* hist2 = new TH1D(Form("hist2_%d", i), Form("%s_TRUEPT", particle_type2), num_pt_bins, pt_bins);

        std::cout << "Created histograms for iteration " << i << " out of " << num_rapidity_bins << "." << std::endl;

        hist1->Sumw2();
        hist2->Sumw2();

        tree1->Draw(Form("%s_TRUEPT >> hist1_%d", particle_type1, i), cuts1);
        tree2->Draw(Form("%s_TRUEPT >> hist2_%d", particle_type2, i), cuts2);

        std::cout << "   Populated histograms from trees for iteration " << i << " out of " << num_rapidity_bins << "." << std::endl;

        TRatioPlot *ratio_plot = new TRatioPlot(hist1, hist2);
        std::cout << "   Created ratio plot for iteration " << i << " out of " << num_rapidity_bins << "." << std::endl;
        ratio_plots.push_back(ratio_plot);
    }

    std::cout << "Done." << std::endl;

    TFile *output_file = new TFile(Form("plotratio_%s_%s.root", particle_type1, particle_type2), "RECREATE");
    if (!output_file || output_file->IsZombie()) {
        std::cerr << Form("Error: Unable to create output file 'plotratio_%s_%s.root'.\n", particle_type1, particle_type2);
        delete output_file;
        delete file1;
        delete file2;
        for (auto& plot : ratio_plots) {
            delete plot;
        }
        return;
    }
    std::cout << "Displaying the ratio plots..." << std::endl;

    output_file->cd();
    TCanvas *c2 = new TCanvas("c2", "Canvas", 1000, 800);
    c2->Divide(2, 3);

    double maxY = -DBL_MAX; 
    double minY = DBL_MAX;

    for (int i = 0; i < 6; ++i) {
        TGraph *graph = ratio_plots[i]->GetCalculationOutputGraph();
        double maxGraphY = graph->GetYaxis()->GetXmax();
        double minGraphY = graph->GetYaxis()->GetXmin();
        if (maxGraphY > maxY) {
            maxY = maxGraphY;
        }
        if (minGraphY < minY) {
            minY = minGraphY;
        }
    }

    for (int i = 0; i < 6; ++i) {
        c2->cd(i + 1);
        TGraph *graph = ratio_plots[i]->GetCalculationOutputGraph();
        auto legend = new TLegend(0.15,0.19,0.35,0.32);
        TString rapidity_range = Form("%.1f < y < %.1f", rapidity_bins[i], rapidity_bins[i+1]);
    	int colors[] = {1, 2, 48, 4, 8, 42};
    	int markers[] = {20, 21, 22, 23, 33, 34};
	int nPoints = graph->GetN();  
	double *x = graph->GetX();
    	double *y = graph->GetY();
    	double *exl = graph->GetEXlow();
    	double *exh = graph->GetEXhigh();
    	double *eyl = graph->GetEYlow();
    	double *eyh = graph->GetEYhigh();
    	double *newX = new double[nPoints];
    	double *newEXlow = new double[nPoints];
    	double *newEXhigh = new double[nPoints];
    	for (int i = 0; i < nPoints; ++i) {
        	newX[i] = x[i] * 0.001;       // Convert MeV/c to GeV/c
        	newEXlow[i] = exl[i] * 0.001; // Convert MeV/c to GeV/c
        	newEXhigh[i] = exh[i] * 0.001; // Convert MeV/c to GeV/c
    	}


    	TGraphAsymmErrors *new_graph = new TGraphAsymmErrors(nPoints, newX, y, newEXlow, newEXhigh, eyl, eyh);
    	new_graph->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    	double xMin = graph->GetXaxis()->GetXmin() * 0.001;
    	double xMax = graph->GetXaxis()->GetXmax() * 0.001;

    	new_graph->GetXaxis()->SetLimits(xMin, xMax);
       new_graph->SetMarkerStyle(markers[i]);
       new_graph->SetMarkerColor(colors[i]);
    	new_graph->Write("graph", TObject::kOverwrite);
	new_graph->SetTitle("");

        // new_graph->GetXaxis()->CenterTitle();
        // new_graph->GetYaxis()->CenterTitle();
        new_graph->GetYaxis()->SetTitleOffset(0.65);
        new_graph->GetYaxis()->SetTitle("D^{0} / D^{+}");
        new_graph->GetXaxis()->SetTitleSize(0.07);
        new_graph->GetYaxis()->SetTitleSize(0.075);
        new_graph->GetXaxis()->SetLabelSize(0.06);
        new_graph->GetYaxis()->SetLabelSize(0.06);
        new_graph->GetYaxis()->SetRangeUser(minY, maxY);
        gPad->SetLeftMargin(0.10);
        gPad->SetRightMargin(0.05);
        gPad->SetBottomMargin(0.18);
        gPad->SetTopMargin(0.02);
        new_graph->Draw("AP"); 
        legend->AddEntry(new_graph, rapidity_range, "elp");
	    legend->SetBorderSize(0);
        legend->SetTextSize(0.07);
        legend->Draw();
        new_graph->Write(Form("graph_%d", i));
    }

    c2->Update();
    c2->Write();
    c2->SaveAs("ratios_D0_Dp_2y5.pdf");
    output_file->Close();
    std::cout << "Done." << std::endl;
    delete output_file;
    delete file1;
    delete file2;
}

