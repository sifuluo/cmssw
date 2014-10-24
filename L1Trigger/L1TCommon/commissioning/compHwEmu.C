void compHwEmu(){

  TFile* inFile = new TFile("l1t.root");

  TH2D* towEtaPhi = (TH2D*)inFile->Get("rawPlots/tower/etaphi");

  TH2D* rawEtaPhi = (TH2D*)inFile->Get("rawPlots/jet/etaphi");
  TH2D* emuEtaPhi = (TH2D*)inFile->Get("simPlots/jet/etaphi");

  TH1D* rawEt = (TH1D*)inFile->Get("rawPlots/jet/et");
  TH1D* emuEt = (TH1D*)inFile->Get("simPlots/jet/et");
  TH1D* rawEta = (TH1D*)inFile->Get("rawPlots/jet/eta");
  TH1D* emuEta = (TH1D*)inFile->Get("simPlots/jet/eta");
  TH1D* rawPhi = (TH1D*)inFile->Get("rawPlots/jet/phi");
  TH1D* emuPhi = (TH1D*)inFile->Get("simPlots/jet/phi");

  TH1D* rawEtTot = (TH1D*)inFile->Get("rawPlots/sum/et");
  TH1D* emuEtTot = (TH1D*)inFile->Get("simPlots/sum/et");
  TH1D* rawEtxTot = (TH1D*)inFile->Get("rawPlots/sum/etx");
  TH1D* emuEtxTot = (TH1D*)inFile->Get("simPlots/sum/etx");
  TH1D* rawEtyTot = (TH1D*)inFile->Get("rawPlots/sum/ety");
  TH1D* emuEtyTot = (TH1D*)inFile->Get("simPlots/sum/ety");

  TH1D* rawHtTot = (TH1D*)inFile->Get("rawPlots/sum/ht");
  TH1D* emuHtTot = (TH1D*)inFile->Get("simPlots/sum/ht");
  TH1D* rawHtxTot = (TH1D*)inFile->Get("rawPlots/sum/htx");
  TH1D* emuHtxTot = (TH1D*)inFile->Get("simPlots/sum/htx");
  TH1D* rawHtyTot = (TH1D*)inFile->Get("rawPlots/sum/hty");
  TH1D* emuHtyTot = (TH1D*)inFile->Get("simPlots/sum/hty");

  TH1D* rawMetPhi = (TH1D*)inFile->Get("rawPlots/sum/metphi");
  TH1D* emuMetPhi = (TH1D*)inFile->Get("simPlots/sum/metphi");
  TH1D* rawMet = (TH1D*)inFile->Get("rawPlots/sum/met");
  TH1D* emuMet = (TH1D*)inFile->Get("simPlots/sum/met");

  TH1D* rawMhtPhi = (TH1D*)inFile->Get("rawPlots/sum/mhtphi");
  TH1D* emuMhtPhi = (TH1D*)inFile->Get("simPlots/sum/mhtphi");
  TH1D* rawMht = (TH1D*)inFile->Get("rawPlots/sum/mht");
  TH1D* emuMht = (TH1D*)inFile->Get("simPlots/sum/mht");

  gStyle->SetPalette(1);

  TCanvas* c1 = new TCanvas("c1","");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(towEtaPhi,"Towers");

  c1->cd();
  towEtaPhi->GetXaxis()->SetTitle("iEta");
  towEtaPhi->GetYaxis()->SetTitle("iPhi");
  towEtaPhi->GetZaxis()->SetTitle("E_{T}");
  towEtaPhi->SetAxisRange(-40.,40.);
  towEtaPhi->Draw("Text");
  leg->Draw();
  c1->Print("compHwEmu.pdf(","pdf");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(emuEtaPhi,"Emulator Jets");

  c1->cd();
  emuEtaPhi->GetXaxis()->SetTitle("iEta");
  emuEtaPhi->GetYaxis()->SetTitle("iPhi");
  emuEtaPhi->SetAxisRange(-40.,40.);
  emuEtaPhi->Draw("text");
  leg->Draw();
  c1->Print("compHwEmu.pdf","pdf");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(rawEtaPhi,"Firmware Jets");

  c1->cd();
  rawEtaPhi->GetXaxis()->SetTitle("iEta");
  rawEtaPhi->GetYaxis()->SetTitle("iPhi");
  rawEtaPhi->SetAxisRange(-40.,40.);
  rawEtaPhi->Draw("text");
  leg->Draw();
  c1->Print("compHwEmu.pdf","pdf");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(rawEt,"Firmware", "l");
  leg->AddEntry(emuEt,"Emulator", "p");

  c1->cd();
  emuEt->SetMarkerStyle(20);
  emuEt->SetMarkerSize(0.4);
  emuEt->SetAxisRange(0.,500.);
  emuEt->GetXaxis()->SetTitle("jet Et");
  emuEt->Draw("p");
  rawEt->Draw("same");
  leg->Draw();
  c1->Print("compHwEmu.pdf","pdf");
  
  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(rawEta,"Firmware", "l");
  leg->AddEntry(emuEta,"Emulator", "p");

  emuEta->SetMarkerStyle(20);
  emuEta->SetMarkerSize(0.4);
  emuEta->SetAxisRange(-40.,40.);
  emuEta->GetXaxis()->SetTitle("jet iEta");
  emuEta->Draw("p");
  rawEta->Draw("same");
  leg->Draw();
  c1->Print("compHwEmu.pdf","pdf");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(rawPhi,"Firmware", "l");
  leg->AddEntry(emuPhi,"Emulator", "p");

  emuPhi->SetMarkerStyle(20);
  emuPhi->SetMarkerSize(0.4);
  emuPhi->GetXaxis()->SetTitle("jet iPhi");
  emuPhi->Draw("p");
  rawPhi->Draw("same");
  leg->Draw();
  c1->Print("compHwEmu.pdf","pdf");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(rawEtTot,"Firmware", "l");
  leg->AddEntry(emuEtTot,"Emulator", "p");

  c1->cd();

  emuEtTot->SetMarkerStyle(20);
  emuEtTot->SetMarkerSize(0.4);
  emuEtTot->GetXaxis()->SetTitle("Total Et");
  emuEtTot->Draw("p");
  rawEtTot->Draw("same");
  leg->Draw();
  c1->Print("compHwEmu.pdf","pdf");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(rawEtxTot,"Firmware", "l");
  leg->AddEntry(emuEtxTot,"Emulator", "p");

  c1->cd();

  emuEtxTot->SetMarkerStyle(20);
  emuEtxTot->SetMarkerSize(0.4);
  emuEtxTot->SetAxisRange(-300.,300.);
  emuEtxTot->GetXaxis()->SetTitle("Met X");
  emuEtxTot->Draw("p");
  rawEtxTot->Draw("same");
  leg->Draw();
  c1->Print("compHwEmu.pdf","pdf");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(rawEtyTot,"Firmware", "l");
  leg->AddEntry(emuEtyTot,"Emulator", "p");

  c1->cd();

  emuEtyTot->SetMarkerStyle(20);
  emuEtyTot->SetMarkerSize(0.4);
  emuEtyTot->SetAxisRange(-300.,300.);
  emuEtyTot->GetXaxis()->SetTitle("Met Y");
  emuEtyTot->Draw("p");
  rawEtyTot->Draw("same");
  leg->Draw();
  c1->Print("compHwEmu.pdf","pdf");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(rawMet,"Firmware", "l");
  leg->AddEntry(emuMet,"Emulator", "p");

  c1->cd();

  emuMet->SetMarkerStyle(20);
  emuMet->SetMarkerSize(0.4);
  emuMet->SetAxisRange(0.,2000.);
  emuMet->GetXaxis()->SetTitle("Met");
  emuMet->Draw("p");
  rawMet->Draw("same");
  leg->Draw();
  c1->Print("compHwEmu.pdf","pdf");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(rawMetPhi,"Firmware", "l");
  leg->AddEntry(emuMetPhi,"Emulator", "p");

  c1->cd();

  emuMetPhi->SetMarkerStyle(20);
  emuMetPhi->SetMarkerSize(0.4);
  emuMetPhi->GetXaxis()->SetTitle("Met phi");
  emuMetPhi->Draw("p");
  rawMetPhi->Draw("same");
  leg->Draw();
  c1->Print("compHwEmu.pdf","pdf");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(rawHtTot,"Firmware", "l");
  leg->AddEntry(emuHtTot,"Emulator", "p");

  c1->cd();

  emuHtTot->SetMarkerStyle(20);
  emuHtTot->SetMarkerSize(0.4);
  emuHtTot->SetAxisRange(0.,50.);
  emuHtTot->GetXaxis()->SetTitle("H_{T}");
  emuHtTot->Draw("p");
  rawHtTot->Draw("same");
  leg->Draw();

  c1->cd();

  emuHtxTot->SetMarkerStyle(20);
  emuHtxTot->SetMarkerSize(0.4);
  emuHtxTot->SetAxisRange(-300.,300.);
  emuHtxTot->GetXaxis()->SetTitle("MHT X");
  emuHtxTot->Draw("p");
  rawEtxTot->Draw("same");
  leg->Draw();
  c1->Print("compHwEmu.pdf","pdf");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(rawEtyTot,"Firmware", "l");
  leg->AddEntry(emuEtyTot,"Emulator", "p");

  c1->cd();

  emuHtyTot->SetMarkerStyle(20);
  emuHtyTot->SetMarkerSize(0.4);
  emuHtyTot->SetAxisRange(-300.,300.);
  emuHtyTot->GetXaxis()->SetTitle("MHT Y");
  emuHtyTot->Draw("p");
  rawHtyTot->Draw("same");
  leg->Draw();
  c1->Print("compHwEmu.pdf","pdf");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(rawMet,"Firmware", "l");
  leg->AddEntry(emuMet,"Emulator", "p");

  c1->cd();

  emuMht->SetMarkerStyle(20);
  emuMht->SetMarkerSize(0.4);
  emuMht->SetAxisRange(0.,2000.);
  emuMht->GetXaxis()->SetTitle("MHT");
  emuMht->Draw("p");
  rawMht->Draw("same");
  leg->Draw();
  c1->Print("compHwEmu.pdf","pdf");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(rawMhtPhi,"Firmware", "l");
  leg->AddEntry(emuMhtPhi,"Emulator", "p");

  c1->cd();

  emuMhtPhi->SetMarkerStyle(20);
  emuMhtPhi->SetMarkerSize(0.4);
  emuMhtPhi->GetXaxis()->SetTitle("MHT phi");
  emuMhtPhi->Draw("p");
  rawMhtPhi->Draw("same");
  leg->Draw();
  c1->Print("compHwEmu.pdf","pdf");

  TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(rawHtTot,"Firmware", "l");
  leg->AddEntry(emuHtTot,"Emulator", "p");

  c1->Print("compHwEmu.pdf)","pdf");

}
