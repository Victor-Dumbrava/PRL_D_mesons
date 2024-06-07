void openInputFiles(std::vector<TGraphAsymmErrors*>& graphs, int mixed = 0, bool Dst = false, bool indirect = false, bool alice = false) {
    // boolean params are: false/true (for D0/Dst), false/true (for ccbar/other sources), false/true (for lchb/alice) respectively

    std::string fileName;
    if (mixed == 0) {
        if (alice) {
            if (indirect) {
                if (Dst) {
                    fileName = "alice_other_plotratio_Dstar_Dp.root";
                } else {
                    fileName = "alice_indirect_from_Dst_D0_Dp.root";
                }
            } else {
                if (Dst) {
                    fileName = "alice_direct_hadronization_plotratio_Dstar_Dp.root";
                } else {
                    fileName = "alice_direct_hadronization_plotratio_D0_Dp.root";
                }
            }
        } else {
            if (indirect) {
                if (Dst) {
                    fileName = "plotratio_other_Dstar_Dp.root";
                } else {
                    fileName = "indirect_from_Dst_D0_Dp.root";
                }
            } else {
                if (Dst) {
                    fileName = "plotratio_direct_hadronization_Dstar_Dp.root";
                } else {
                    fileName = "plotratio_direct_hadronization_D0_Dp.root";
                }
            }
        }
    }
    else {
        if (alice) {
            fileName = Form("alice_mixed%i_plotratio_D0_Dp.root", mixed);
        } else {
            fileName = Form("mixed%i_D0_Dp.root", mixed);
        }
    }

    auto file = new TFile(fileName.c_str(), "READ");

    if (!file || file->IsZombie()) {
        std::cerr << "Error: Could not open input file " << fileName << "." << std::endl;
        delete file; 
        file = nullptr;
        return;
    }

    int numGraphs = (alice ? 1 : 2);

    for (size_t i = 0; i < numGraphs; ++i) {
        std::string graphName = "graph" + (alice ? "" : "_" + std::to_string(i));
        TGraphAsymmErrors* newGraph = (TGraphAsymmErrors*)file->Get(graphName.c_str());
        graphs.push_back(newGraph);
    }

    std::cout << "Input file " << fileName << " read successfully." << std::endl;
}

void mixed_alice_D0_Dp_comparison() {

    std::vector<TGraphAsymmErrors*> graphs;
    std::vector<TString> labels = { "0.0 < y < 1.0 (D^{0}: D^{*+}, D^{+}: c / #bar{c})",
                                    "0.0 < y < 1.0 (D^{0}: D^{*+} or D^{*0}, D^{+}: c / #bar{c})",
                                    "0.0 < y < 1.0 (D^{0}: c / #bar{c}, D^{+}: D^{*+} or D^{*0})"};
    openInputFiles(graphs, 3, false, false, true);
    openInputFiles(graphs, 2, false, false, true);
    openInputFiles(graphs, 1, false, false, true);
    // std::vector<TString> labels =  {"2.0 < y < 2.5 (from c or #bar{c})",
    //                                 "2.5 < y < 3.0 (from c or #bar{c})",
    //                                 "0.0 < y < 1.0 (from c or #bar{c})",
    //                                 "2.0 < y < 2.5 (from D^{*0} or D^{*+})",
    //                                 "2.5 < y < 3.0 (from D^{*0} or D^{*+})",
    //                                 "0.0 < y < 1.0 (from D^{*0} or D^{*+})"}; 

    // boolean params are: false/true (for D0/Dst), false/true (for ccbar/other sources), false/true (for lchb/alice) respectively
    // openInputFiles(graphs, false, false, false);
    // openInputFiles(graphs, false, false, true);
    // openInputFiles(graphs, false, true, false);
    // openInputFiles(graphs, false, true, true);

    std::cout << "Starting processing." << std::endl;

    TFile *output_file = new TFile("comparison_mixed_alice_D0_Dp.root", "RECREATE");
    if (!output_file || output_file->IsZombie()) {
        std::cerr << "Error: Unable to create output file 'comparison_mixed_alice_D0_Dp.root'.\n";
        delete output_file;
        return;
    }

    output_file->cd();
    TCanvas *c1 = new TCanvas();
    TLegend *legend = new TLegend(0.1, 0.42, 0.55, 0.67);
    legend->SetTextSize(0.03);

    if (!graphs.empty()) {
        graphs[0]->Draw("AP"); 
        graphs[0]->GetYaxis()->SetTitle("D^{0} / D^{+}");
        graphs[0]->GetXaxis()->SetTitleSize(0.04);
        graphs[0]->GetYaxis()->SetTitleSize(0.04);
        graphs[0]->GetXaxis()->SetLabelSize(0.04);
        graphs[0]->GetYaxis()->SetLabelSize(0.04);
        graphs[0]->GetXaxis()->SetTitleOffset(1);
        graphs[0]->GetYaxis()->SetTitleOffset(1);
        graphs[0]->GetYaxis()->SetRangeUser(0.1, 2.4);
        graphs[0]->SetTitle("Ratio of D^{0} / D^{+} for the different sources and rapidity ranges");
        gPad->SetLeftMargin(0.10);
        gPad->SetRightMargin(0.10);
        gPad->SetBottomMargin(0.15);
        gPad->SetTopMargin(0.10);
        graphs[0]->SetMarkerStyle(20);
        graphs[0]->SetMarkerColor(1);
        graphs[0]->SetLineColor(1);
        graphs[0]->SetLineWidth(2);
        legend->AddEntry(graphs[0], labels[0], "elp"); 
    }

    for (size_t i = 1; i < graphs.size(); ++i) {
        graphs[i]->Draw("P SAME"); 
        graphs[i]->SetMarkerStyle(20 + i);
        graphs[i]->SetMarkerColor(i != 4 ? i + 1 : 7);
        graphs[i]->SetLineColor(i != 4 ? i + 1 : 7);
        graphs[i]->SetLineWidth(2);
        legend->AddEntry(graphs[i], labels[i], "elp");
    }

    legend->Draw();
    c1->Update();
    c1->Write();
    c1->SaveAs("comparison_mixed_alice_D0_Dp.pdf");
    output_file->Close();
    delete output_file;

    for (auto graph : graphs) {
        delete graph;
    }

    delete legend;
    delete c1;

    std::cout << "Done." << std::endl;

}
