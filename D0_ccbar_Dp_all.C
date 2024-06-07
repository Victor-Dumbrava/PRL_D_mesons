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
        if (Dst) {
            if (alice) {
                fileName = Form("alice_mixed%i_plotratio_Dstar_Dp.root", mixed);
            } else {
                fileName = Form("mixed%i_Dstar_Dp.root", mixed);
            }
        }
        else {
            if (alice) {
                fileName = Form("alice_mixed%i_plotratio_D0_Dp.root", mixed);
            } else {
                fileName = Form("mixed%i_D0_Dp.root", mixed);
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

    int numGraphs = (alice ? 1 : 2);

    for (size_t i = 0; i < numGraphs; ++i) {
        std::string graphName = "graph" + (alice ? "" : "_" + std::to_string(i));
        TGraphAsymmErrors* newGraph = (TGraphAsymmErrors*)file->Get(graphName.c_str());
        graphs.push_back(newGraph);
    }

    std::cout << "Input file " << fileName << " read successfully." << std::endl;
}

void D0_ccbar_Dp_all() {

    // std::vector<TGraphAsymmErrors*> graphs;
    // std::vector<TString> labels = { "2.0 < y < 2.5 (D^{*+}: other, D^{+}: c / #bar{c})",
    //                                 "2.5 < y < 3.0 (D^{*+}: other, D^{+}: c / #bar{c})",
    //                                 "2.0 < y < 2.5 (D^{*+}: c / #bar{c}, D^{+}: D^{*0} or D^{*+})",
    //                                 "2.5 < y < 3.0 (D^{*+}: c / #bar{c}, D^{+}: D^{*0} or D^{*+})",
    //                                 "0.0 < y < 1.0 (D^{*+}: other, D^{+}: c / #bar{c})",
    //                                 "0.0 < y < 1.0 (D^{*+}: c / #bar{c}, D^{+}: D^{*0} or D^{*+})"};
    // openInputFiles(graphs, 1, true, false, false);
    // openInputFiles(graphs, 2, true, false, false);
    // openInputFiles(graphs, 1, true, false, true);
    // openInputFiles(graphs, 2, true, false, true);

    std::vector<TGraphAsymmErrors*> graphs;
    // std::vector<TString> labels = {
    //     "0.0 < y < 1.0 (D^{+} from c / #bar{c})",
    //     "2.0 < y < 2.5 (D^{+} from c / #bar{c})",
    //     "2.5 < y < 3.0 (D^{+} from c / #bar{c})",
    //     "0.0 < y < 1.0 (D^{+} from D^{*0} or D^{*+})",
    //     "2.0 < y < 2.5 (D^{+} from D^{*0} or D^{*+})",
    //     "2.5 < y < 3.0 (D^{+} from D^{*0} or D^{*+})",
    // };
    std::vector<TString> labels = {
        "0.0 < y < 1.0",
        "2.0 < y < 2.5",
        "2.5 < y < 3.0",
        "0.0 < y < 1.0",
        "2.0 < y < 2.5",
        "2.5 < y < 3.0"
    };
    openInputFiles(graphs, 0, false, false, true);
    openInputFiles(graphs, 0, false, false, false);
    openInputFiles(graphs, 1, false, false, true);
    openInputFiles(graphs, 1, false, false, false);
    
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

    TFile *output_file = new TFile("D0_ccbar_Dp_all.root", "RECREATE");
    if (!output_file || output_file->IsZombie()) {
        std::cerr << "Error: Unable to create output file 'D0_ccbar_Dp_all.root'.\n";
        delete output_file;
        return;
    }

    output_file->cd();

    Double_t w = 1200;
    Double_t h = 300;
    TCanvas *c1 = new TCanvas("c1", "c1", w, h);
    c1->SetWindowSize(w + (w - c1->GetWw()), h + (h - c1->GetWh()));


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

    int colors[] = {1, 2, 4};
    int markers[] = {20, 21, 22};
    gStyle->SetTitleFontSize(0.06); 

    c1->Divide(2, 1);
    c1->cd(1);
    
    TLegend *legend = new TLegend(0.12, 0.65, 0.32, 0.85);
    legend->SetTextSize(0.04);
    c1->cd(2);
    TLegend *legend2 = new TLegend(0.12, 0.65, 0.32, 0.85);
    legend2->SetTextSize(0.04);
    c1->cd(1);
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
        gPad->SetLeftMargin(0.10);
        gPad->SetRightMargin(0.10);
        gPad->SetBottomMargin(0.15);
        gPad->SetTopMargin(0.12);
        new_graph->GetXaxis()->SetLimits(xMin, xMax);
        new_graph->SetMarkerStyle(markers[0]);
        new_graph->SetMarkerColor(colors[0]);
        new_graph->SetLineColor(colors[0]);
        new_graph->SetLineWidth(2);
        new_graph->GetXaxis()->SetTitleSize(0.06);
        new_graph->GetYaxis()->SetTitleSize(0.06);
        new_graph->GetXaxis()->SetLabelSize(0.05);
        new_graph->GetYaxis()->SetLabelSize(0.05);
        new_graph->GetYaxis()->SetRangeUser(0.2, 0.5);
        new_graph->Draw("AP"); 
        legend->AddEntry(new_graph, labels[0], "elp");
        legend->SetBorderSize(0);
        legend->SetTextSize(0.05);
        new_graph->SetTitle("D^{0} and D^{+} from c or #bar{c}");
        new_graph->GetYaxis()->SetTitleOffset(0.8);
        new_graph->GetXaxis()->SetTitleOffset(0);
        new_graph->GetYaxis()->SetTitle("D^{0} / D^{+}");
    }

    for (size_t i = 1; i < graphs.size(); ++i) {
        if (i == 3) {
            legend->Draw();
            c1->cd(2);
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
            gPad->SetLeftMargin(0.10);
            gPad->SetRightMargin(0.10);
            gPad->SetBottomMargin(0.15);
            gPad->SetTopMargin(0.12);
            new_graph->GetXaxis()->SetLimits(xMin, xMax);
            new_graph->SetMarkerStyle(markers[i % 3]);
            new_graph->SetMarkerColor(colors[i % 3]);
            new_graph->SetLineColor(colors[i % 3]);
            new_graph->GetXaxis()->SetTitleSize(0.06);
            new_graph->GetYaxis()->SetTitleSize(0.06);
            new_graph->GetXaxis()->SetLabelSize(0.05);
            new_graph->GetYaxis()->SetLabelSize(0.05);
            new_graph->SetLineWidth(2);
            new_graph->GetYaxis()->SetRangeUser(0.2, 0.5);
            new_graph->Draw("AP"); 
            new_graph->SetTitle("D^{0} from c or #bar{c},    D^{+} from feeddown D^{*+}");
            new_graph->GetYaxis()->SetTitleOffset(0.8);
            new_graph->GetXaxis()->SetTitleOffset(0);
            new_graph->GetYaxis()->SetTitle("D^{0} / D^{+}");
            legend2->AddEntry(new_graph, labels[i], "elp"); 
            legend2->SetBorderSize(0);
            legend2->SetTextSize(0.05);
        }
        else {
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
            new_graph->SetMarkerStyle(markers[i % 3]);
            new_graph->SetMarkerColor(colors[i % 3]);
            new_graph->SetLineColor(colors[i % 3]);
            new_graph->SetLineWidth(2);
            new_graph->Draw("P SAME"); 
            if (i < 3) {
                legend->AddEntry(new_graph, labels[i], "elp"); 
            }
            else { 
                legend2->AddEntry(new_graph, labels[i], "elp"); 
            }
        }
    }

    legend2->Draw();
    c1->Update();
    c1->Write();
    c1->Draw();
    c1->SaveAs("D0_ccbar_Dp.pdf");
    output_file->Close();
    delete output_file;

    std::cout << "Done." << std::endl;

}

