#include "L1Trigger/L1TMuonEndCap/interface/EndCapParamsHelper.h"

#include <iostream>

using namespace l1t;
using namespace std;

const EndCapParamsHelper *  EndCapParamsHelper::readFromEventSetup(const L1TMuonEndCapParams * es){
  return new EndCapParamsHelper(es);
}

EndCapParamsHelper *  EndCapParamsHelper::readAndWriteFromEventSetup(const L1TMuonEndCapParams * es){
  EndCapParamsHelper * x = new EndCapParamsHelper(es);
  x->useCopy();
  return x;
}

EndCapParamsHelper::EndCapParamsHelper(L1TMuonEndCapParams * w) {
  write_ = w; 
  check_write(); 
  we_own_write_ = false;
  //write_->m_version = VERSION; 
  read_ = write_; 
}

EndCapParamsHelper::EndCapParamsHelper(const L1TMuonEndCapParams * es) {read_ = es; write_=NULL;}

void EndCapParamsHelper::useCopy(){
  write_ = new L1TMuonEndCapParams(*read_);
  we_own_write_ = true;
  read_  = write_;
}

EndCapParamsHelper::~EndCapParamsHelper() {
  if (we_own_write_ && write_) delete write_;
}


// print all the L1 GT stable parameters
void EndCapParamsHelper::print(std::ostream& myStr) const {
    myStr << "\nL1T EndCap  Parameters \n" << std::endl;
}


void EndCapParamsHelper::loadTreeFromXML(const char* filename)
{   
    // First create the engine.
    TXMLEngine* xml = new TXMLEngine();

    // Now try to parse xml file.
    XMLDocPointer_t xmldoc = xml->ParseFile(filename);
    if (xmldoc==0)
    {
        delete xml;
        return;  
    }


    // Get access to main node of the xml file.
    XMLNodePointer_t mainnode = xml->DocGetRootElement(xmldoc);
   
    DTree dtree;
    dtree.push_back(DTreeNode());
    // Recursively connect nodes together.
    loadTreeFromXMLRecursive(xml, mainnode, dtree, 0);
    cout << "DEBUG:  new tree created with size " << dtree.size() << "\n";

    std::vector<double> evt = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    cout << "DEBUG: evalution test:  " << evalTree(evt, dtree) << "\n";


    //HACK for testing:
    dtree_test = dtree;

    // Release memory before exit
    xml->FreeDoc(xmldoc);
    delete xml;
}

void EndCapParamsHelper::loadTreeFromXMLRecursive(TXMLEngine* xml, XMLNodePointer_t xnode, DTree & tree, unsigned index) 
{
  assert(tree.size() > index); 
  cout << "DEBUG:  recursive call at index " << index <<"\n";

  // Get the split information from xml.
  XMLAttrPointer_t attr = xml->GetFirstAttr(xnode);
  std::vector<std::string> splitInfo(3);
  for(unsigned int i=0; i<3; i++)
    {
      splitInfo[i] = xml->GetAttrValue(attr); 
      attr = xml->GetNextAttr(attr);  
    }
  
  // Convert strings into numbers.
  std::stringstream converter;
  Int_t splitVar;
  Double_t splitVal;
  Double_t fitVal;  

  converter << splitInfo[0];
  converter >> splitVar;
  converter.str("");
  converter.clear();
  
  converter << splitInfo[1];
  converter >> splitVal;
  converter.str("");
  converter.clear();
  
  converter << splitInfo[2];
  converter >> fitVal;
  converter.str("");
  converter.clear();
  
  cout << "fitval:  " << fitVal << "\n";

  // Store gathered splitInfo into the node object.
  tree[index].splitVar = splitVar;
  tree[index].splitVal = splitVal;
  tree[index].fitVal   = fitVal;

  // Get the xml daughters of the current xml node. 
  XMLNodePointer_t xleft = xml->GetChild(xnode);
  XMLNodePointer_t xright = xml->GetNext(xleft);

  // This seems potentially problematic, but leaving for now until
  // bitwise equivalence is demonstrated...
  if(xleft == 0 || xright == 0) return;

  // append two more nodes at end of tree and update this indices in this node:
  tree[index].ileft  = tree.size();
  tree[index].iright = tree[index].ileft + 1;
  tree.push_back(DTreeNode());
  tree.push_back(DTreeNode());

  // recursively handle the next two nodes:
  loadTreeFromXMLRecursive(xml, xleft, tree, tree[index].ileft);
  loadTreeFromXMLRecursive(xml, xright, tree, tree[index].iright);
}

double EndCapParamsHelper::evalTreeRecursive(std::vector<double> & data, DTree & tree, int index){
  DTreeNode & node = tree[index];
  if ((node.ileft == 0) && (node.iright==0)){
    return node.fitVal;
  }
  assert(data.size() > (unsigned) node.splitVar);
  // by convention, nodes are either not terminal or fully terminal
  assert(node.ileft > 0);
  assert(node.iright > 0);
  
  if (data[node.splitVar] < node.splitVal){
    return evalTreeRecursive(data, tree, tree[index].ileft);
  } else {
    return evalTreeRecursive(data, tree, tree[index].iright);
  }
}
