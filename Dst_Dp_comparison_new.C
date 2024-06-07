void openInputFiles(std::vector<TGraphAsymmErrors*>& graphs, bool Dst = false, bool indirect = false, bool alice = false) {
    // boolean params are: false/true (for D0/Dst), false/true (for ccbar/other sources), false/true (for lchb/alice) respectively

    std::string fileName;
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

    auto file = new TFile(fileName.c_str(), "READ");

    if (!file || file->IsZombie()) {
        std::cerr << "Error: Could not open input file " << fileName << "." << std::endl;
        delete file; 
        file = nullptr;
        return;
    }

    int numGraphs = (alice ? 1 : 6);

    for (size_t i = 0; i < numGraphs; ++i) {
        std::string graphName = "graph" + (alice ? "" : "_" + std::to_string(i));
        TGraphAsymmErrors* newGraph = (TGraphAsymmErrors*)file->Get(graphName.c_str());
        graphs.push_back(newGraph);
    }

    std::cout << "Input file " << fileName << " read successfully." << std::endl;
}

void Dst_Dp_comparison_new() {

    std::vector<TGraphAsymmErrors*> graphs;
    std::vector<TString> labels =  {"2.0 < y < 2.5 (from c or #bar{c})",
                                    "2.5 < y < 3.0 (from c or #bar{c})",
                                    "3.0 < y < 3.5 (from c or #bar{c})",
                                    "3.5 < y < 4.0 (from c or #bar{c})",
                                    "4.0 < y < 4.5 (from c or #bar{c})",
                                    "4.5 < y < 5.0 (from c or #bar{c})",
                                    "0.0 < y < 1.0 (from c or #bar{c})",
                                    "2.0 < y < 2.5 (other sources)",
                                    "2.5 < y < 3.0 (other sources)",
                                    "3.0 < y < 3.5 (other sources)",
                                    "3.5 < y < 4.0 (other sources)",
                                    "4.0 < y < 4.5 (other sources)",
                                    "4.5 < y < 5.0 (other sources)",
                                    "0.0 < y < 1.0 (other sources)"}; 
    std::vector<TString> r_ranges =  {"2.0 < y < 2.5",
                                    "2.5 < y < 3.0",
                                    "3.0 < y < 3.5",
                                    "3.5 < y < 4.0",
                                    "4.0 < y < 4.5",
                                    "4.5 < y < 5.0",
                                    "0.0 < y < 1.0"}; 

    // boolean params are: false/true (for D0/Dst), false/true (for ccbar/other sources), false/true (for lchb/alice) respectively
    openInputFiles(graphs, true, false, false); // Dst, ccbar
    openInputFiles(graphs, true, false, true);
    openInputFiles(graphs, true, true, false);
    openInputFiles(graphs, true, true, true);

    std::cout << "Starting processing." << std::endl;

    TFile *output_file = new TFile("comparison_Dst_Dp_new.root", "RECREATE");
    if (!output_file || output_file->IsZombie()) {
        std::cerr << "Error: Unable to create output file 'comparison_Dst_Dp_new.root'.\n";
        delete output_file;
        return;
    }

    output_file->cd();
    TCanvas *c1 = new TCanvas();
    TLegend *legend = new TLegend(0.15, 0.6, 0.45, 0.85);
    legend->SetTextSize(0.03);

    double maxY = -DBL_MAX; 
    double minY = DBL_MAX;

    for (int i = 0; i < graphs.size(); ++i) {
        TGraph *graph = graphs[i];
        double maxGraphY = graph->GetYaxis()->GetXmax();
        double minGraphY = graph->GetYaxis()->GetXmin();
        if (maxGraphY > maxY) {
            maxY = maxGraphY;
        }
        if (minGraphY < minY) {
            minY = minGraphY;
        }
    }
    int colors[] = {1, 2, 48, 4, 8, 42, 40};
    int markers[] = {20, 21, 22, 23, 33, 34, 35};

    if (!graphs.empty()) {
        TGraph *graph = graphs[0];
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
        new_graph->SetMarkerStyle(markers[0]);
        new_graph->SetMarkerColor(colors[0]);
        new_graph->SetLineColor(colors[0]);
        new_graph->SetLineWidth(2);
        new_graph->SetTitle("Ratio of D^{*+} / D^{+} for the different sources and rapidity ranges");
        new_graph->GetYaxis()->SetTitleOffset(0);
        new_graph->GetXaxis()->SetTitleOffset(0);
        new_graph->GetYaxis()->SetTitle("D^{*+} / D^{+}");
        new_graph->GetXaxis()->SetTitleSize(0.04);
        new_graph->GetYaxis()->SetTitleSize(0.04);
        // new_graph->GetXaxis()->SetLabelSize(0.06);
        // new_graph->GetYaxis()->SetLabelSize(0.06);
        new_graph->GetYaxis()->SetRangeUser(minY - 0.1, maxY + 0.1);
        new_graph->Draw("AP"); 
        legend->AddEntry(new_graph, r_ranges[0], "elp");
        legend->SetBorderSize(0);
        // legend->SetTextSize(0.07);
    }

    for (size_t i = 1; i < graphs.size(); ++i) {
    // for (size_t i = 1; i < 7; ++i) {
        TGraph *graph = graphs[i];
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
        new_graph->SetMarkerStyle(markers[i % 7]);
        new_graph->SetMarkerColor(colors[i % 7]);
        new_graph->GetXaxis()->SetLimits(xMin, xMax);
        new_graph->SetLineColor(colors[i % 7]);
        new_graph->SetLineWidth(2);
        new_graph->Draw("P SAME"); 
        if (i <= 6) {
            legend->AddEntry(new_graph, r_ranges[i], "elp");
        }
    }

    TLatex *t_ccbar = new TLatex(2.0,1.8,"#bf{from c or #bar{c}}");
    TText *t_other = new TLatex(2.0,.5,"#bf{from other sources}");
    t_ccbar->SetTextSize(0.035);
    t_other->SetTextSize(0.035);
    // t_ccbar->SetTextAlign(22);
    // t->SetTextColor(kBlack);
    // t->SetTextFont(43);
    // t->SetTextSize(40);
    // t_ccbar->SetTextAngle(0);
    t_ccbar->Draw();
    t_other->Draw();

    legend->Draw();
    c1->Update();
    c1->Write();
    c1->Draw();
    c1->SaveAs("comparison_Dst_Dp_new.pdf");
    output_file->Close();
    delete output_file;

    // for (auto graph : graphs) {
    //     delete graph;
    // }

    // delete legend;
    // delete c1;

    std::cout << "Done." << std::endl;
}

