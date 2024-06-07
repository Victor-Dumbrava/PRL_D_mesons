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

void alice_mixed3_D0_Dp() {
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

    int num_rapidity_bins = 1;
    double rapidity_min = 0.0;
    double rapidity_max = 1.0;

    int num_pt_bins = 12;
    double pt_bins[] = {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 12000, 14000};

    std::cout << "Calculating the ratio plot..." << std::endl;

    TCut cuts1 = Form("%s_TRUEY >= 0.0 && %s_TRUEY < 1.0 && abs(Km_MC_MOTHER_ID) == 421 && abs(Hp_MC_MOTHER_ID) == 421  && ((abs(%s_MC_MOTHER_ID) == 413))", particle_type1, particle_type1, particle_type1);
    TCut cuts2 = Form("%s_TRUEY >= 0.0 && %s_TRUEY < 1.0 && abs(Km_MC_MOTHER_ID) == 411 && abs(H1_MC_MOTHER_ID) == 411 && abs(H2_MC_MOTHER_ID) == 411 && abs(%s_MC_MOTHER_ID) == 4", particle_type2, particle_type2, particle_type2);

    std::cout << "    Cuts made." << std::endl;

    TH1D* hist1 = new TH1D("hist1", Form("%s_TRUEPT", particle_type1), num_pt_bins, pt_bins);
    TH1D* hist2 = new TH1D("hist2", Form("%s_TRUEPT", particle_type2), num_pt_bins, pt_bins);

    std::cout << "Created histograms." << std::endl;

    hist1->Sumw2();
    hist2->Sumw2();

    tree1->Draw(Form("%s_TRUEPT >> hist1", particle_type1), cuts1);
    tree2->Draw(Form("%s_TRUEPT >> hist2", particle_type2), cuts2);

    std::cout << "Populated histogram." << std::endl;

    TRatioPlot *ratio_plot = new TRatioPlot(hist1, hist2);
    std::cout << "Created ratio plot." << std::endl;

    TFile *output_file = new TFile(Form("alice_mixed3_plotratio_%s_%s.root", particle_type1, particle_type2), "RECREATE");
    if (!output_file || output_file->IsZombie()) {
        std::cerr << Form("Error: Unable to create output file 'alice_mixed3_plotratio_%s_%s.root'.\n", particle_type1, particle_type2);
        delete output_file;
        delete file1;
        delete file2;
        delete ratio_plot;
        return;
    }
    std::cout << "Displaying the ratio plot..." << std::endl;

    output_file->cd();
    TCanvas *c2 = new TCanvas("c2", "Canvas");

    TGraph *graph = ratio_plot->GetCalculationOutputGraph();
    auto legend = new TLegend(0.14,0.15,0.32,0.25);
    TString rapidity_range = "0.0 < y < 1.0";
    graph->GetXaxis()->SetTitle("p_{T} (MeV/c)");
    graph->GetXaxis()->CenterTitle();
    graph->GetYaxis()->CenterTitle();
    graph->GetYaxis()->SetTitle("D^{0} / D^{+}");
    graph->SetTitle("D^{0} from D^{*+}, D^{+} from c or #bar{c}");
    graph->SetMarkerStyle(20);
    graph->SetMarkerColor(1);
    graph->GetXaxis()->SetTitleSize(0.04);
    graph->GetYaxis()->SetTitleSize(0.04);
    // graph->GetXaxis()->SetLabelSize(0.05);
    // graph->GetYaxis()->SetLabelSize(0.05);
    graph->Draw("AP"); 
    legend->AddEntry(graph, rapidity_range, "elp");
    legend->Draw();
    graph->Write("graph");
    c2->Update();
    c2->Write();
    c2->SaveAs("ratio_mixed3_D0_Dp_0y1.pdf");
    output_file->Close();
    
    std::cout << "Done." << std::endl;
    delete output_file;
    delete file1;
    delete file2;
}

