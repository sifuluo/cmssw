#include "CondTools/L1TriggerExt/interface/L1ConfigOnlineProdBaseExt.h"

//#include "L1Trigger/L1TCalorimeter/interface/L1TCaloParamsStage1Builder.h"

#include "CondFormats/L1TObjects/interface/CaloConfig.h"
#include "CondFormats/L1TObjects/interface/CaloParams.h"
#include "CondFormats/DataRecord/interface/L1TCaloConfigRcd.h"
#include "CondFormats/DataRecord/interface/L1TCaloParamsStage1HIRcd.h"
#include "CaloParamsHelper.h"

class L1TCaloParamsStage1HIOnlineProd : public L1ConfigOnlineProdBaseExt<L1TCaloParamsStage1HIRcd,l1t::CaloParams> {
private:
///  // default parameters, that are not present in OMDS but have to be set for the emulator:
///  std::vector<int>    etSumEtaMin, etSumEtaMax;
///  std::vector<double> etSumEtThreshold;
///  double towerLsbSum, regionLsb, jetLsb, egLsb;
  static void hexStringToVector(const std::string& s, std::vector<uint32_t>& v);

public:
  virtual boost::shared_ptr<l1t::CaloParams> newObject(const std::string& objectKey) override ;

  L1TCaloParamsStage1HIOnlineProd(const edm::ParameterSet&);
  ~L1TCaloParamsStage1HIOnlineProd(void){}
};

L1TCaloParamsStage1HIOnlineProd::L1TCaloParamsStage1HIOnlineProd(const edm::ParameterSet& iConfig) :
  L1ConfigOnlineProdBaseExt<L1TCaloParamsStage1HIRcd,l1t::CaloParams>(iConfig) {
///  // fetch the default parameters from the configuration of this producer
///  etSumEtaMin      = iConfig.getParameter< std::vector<int> >("etSumEtaMin");
///  etSumEtaMax      = iConfig.getParameter< std::vector<int> >("etSumEtaMax");
///  etSumEtThreshold = iConfig.getParameter< std::vector<double> >("etSumEtThreshold");
///  towerLsbSum      = iConfig.getParameter<double>("towerLsbSum");
///  regionLsb        = iConfig.getParameter<double>("regionLsb");
///  jetLsb           = iConfig.getParameter<double>("jetLsb");
///  egLsb            = iConfig.getParameter<double>("egLsb");
}

void L1TCaloParamsStage1HIOnlineProd::hexStringToVector(const std::string& s, std::vector<uint32_t>& v)
{
  unsigned int len = s.length();
  // TODO: XCEPT_ASSERT((len % 8) == 0, tsexception::ConversionError, "Input string length isn't power of 8");

  v.clear();
  for (unsigned int i = 0; i < len; i += 8) {
    uint32_t x;
    std::stringstream ss;
    ss << std::hex << s.substr(i, 8);
    ss >> x;
    v.push_back(x);
  }
}

boost::shared_ptr<l1t::CaloParams> L1TCaloParamsStage1HIOnlineProd::newObject(const std::string& objectKey) {
  // Note: Key should be MP7_HI_CONF_KEY
  using namespace edm::es;

  std::string stage1CaloSchema = "CMS_S1CALOL2" ;
  edm::LogInfo( "L1-O2O: L1TCaloParamsStage1HIOnlineProd" ) << "Producing l1t::CaloParams with key =" << objectKey;

  //boost::shared_ptr< l1t::CaloParams > caloParams = L1TCaloParamsStage1Builder::build(defaultSettings);
  boost::shared_ptr< l1t::CaloParams > caloParams( new l1t::CaloParams() );
  l1t::CaloParamsHelper *m_params_helper =  new l1t::CaloParamsHelper();

  if (objectKey.empty()) {
    edm::LogInfo( "L1-O2O: L1TCaloParamsStage1HIOnlineProd" ) << "Key is empty, returning empty CaloParams";
    return caloParams;
  }

  // Get the keys to all subtables inside PP configuration
  std::vector< std::string > queryColumns;
  queryColumns.push_back( "HI_ALGO_REGS_KEY" ) ;
  queryColumns.push_back( "CENTRALITY_LUT_KEY" ) ;
  queryColumns.push_back( "EXT_CENTRALITY_LUT_KEY" ) ;
  queryColumns.push_back( "HF_ENERGIES_THR_LUT_KEY" ) ;

  l1t::OMDSReader::QueryResults mp7_hi_conf =
    m_omdsReader.basicQuery( queryColumns,
			     stage1CaloSchema,
			     "MP7_HI_CONF",
			     "MP7_HI_CONF.MP7_HI_CONF_KEY",
			     m_omdsReader.singleAttribute(objectKey)
      ) ;

  if( mp7_hi_conf.queryFailed() || mp7_hi_conf.numberRows() != 1 ){
    edm::LogError( "L1-O2O: L1TCaloParamsStage1HIOnlineProd" ) << "Cannot get MP7_HI_CONF" ;
    return boost::shared_ptr< l1t::CaloParams >() ;
  }

  std::string hi_algo_regs_key, centrality_lut_key, ext_centrality_lut_key, hf_energies_thr_lut_key;
  mp7_hi_conf.fillVariable( "HI_ALGO_REGS_KEY", hi_algo_regs_key) ;
  mp7_hi_conf.fillVariable( "CENTRALITY_LUT_KEY", centrality_lut_key);
  mp7_hi_conf.fillVariable( "EXT_CENTRALITY_LUT_KEY", ext_centrality_lut_key) ;
  mp7_hi_conf.fillVariable( "HF_ENERGIES_THR_LUT_KEY", hf_energies_thr_lut_key) ;

  // Variables will be used later
  int jet_seed_thr, ht_sum_thr, centrality_eta_mask, eg_eta_cut, phi_ring_eta_masking, single_track_eta_cut;
  if (!hi_algo_regs_key.empty()) {
    queryColumns.clear();
    queryColumns.push_back( "JET_SEED_THR" ) ;          // Jet seed threshold
    queryColumns.push_back( "HT_SUM_THR" ) ;            // HT sum threshold
    queryColumns.push_back( "CENTRALITY_ETA_MASK" ) ;
    queryColumns.push_back( "EG_ETA_CUT" ) ;
    queryColumns.push_back( "PHI_RING_ETA_MASKING" ) ;
    queryColumns.push_back( "SINGLE_TRACK_ETA_CUT" ) ;

    // select queryColumns from CMS_S1CALOL2.PP_ALGO_REGS where PP_ALGO_REGS_KEY = pp_algo_regs_key ;
    l1t::OMDSReader::QueryResults hi_algo_regs =
      m_omdsReader.basicQuery( queryColumns,
			       stage1CaloSchema,
			       "HI_ALGO_REGS",
			       "HI_ALGO_REGS.HI_ALGO_REGS_KEY",
			       m_omdsReader.singleAttribute(hi_algo_regs_key)
	) ;

    if( hi_algo_regs.queryFailed() || hi_algo_regs.numberRows() != 1 ){
      edm::LogError( "L1-O2O: L1TCaloParamsStage1HIOnlineProd" ) << "Cannot get HI_ALGO_REGS" ;
      return boost::shared_ptr< l1t::CaloParams >() ;
    }

    hi_algo_regs.fillVariable( "JET_SEED_THR",        jet_seed_thr);
    hi_algo_regs.fillVariable( "HT_SUM_THR",          ht_sum_thr);
    hi_algo_regs.fillVariable( "CENTRALITY_ETA_MASK", centrality_eta_mask);
    hi_algo_regs.fillVariable( "EG_ETA_CUT",          eg_eta_cut);
    hi_algo_regs.fillVariable( "PHI_RING_ETA_MASKING", phi_ring_eta_masking);
    hi_algo_regs.fillVariable( "SINGLE_TRACK_ETA_CUT", single_track_eta_cut);

    // Parse all energy registers that are in 0.5GeV steps
    // TODO: Put energy_thr (pileup threshold) to proper place
    m_params_helper->setJetSeedThreshold(jet_seed_thr/2.);
    m_params_helper->setEtSumEtThreshold(0, 0); // Online hard-codes this
    m_params_helper->setEtSumEtThreshold(1, ht_sum_thr/2.);
    m_params_helper->setCentralityRegionMask(centrality_eta_mask);
    m_params_helper->setEgEtaCut(eg_eta_cut);
    m_params_helper->setJetRegionMask(phi_ring_eta_masking);
    m_params_helper->setTauRegionMask(single_track_eta_cut);

  } else {
    // Registers skipped
  }

  if (!hf_energies_thr_lut_key.empty()) {

    queryColumns.clear();
    queryColumns.push_back( "DATA" ) ;            // Hex LUT data
    l1t::OMDSReader::QueryResults hfThrLUT = m_omdsReader.basicQuery( queryColumns,
								      stage1CaloSchema,
								      "HF_ENERGIES_THR_LUT",
								      "HF_ENERGIES_THR_LUT.HF_ENERGIES_THR_LUT_KEY",
								      m_omdsReader.singleAttribute(hf_energies_thr_lut_key)
      ) ;

    if( hfThrLUT.queryFailed() || hfThrLUT.numberRows() != 1){
      edm::LogError( "L1-O2O: L1TCaloParamsStage1HIOnlineProd" ) << "Cannot get PILEUP_SUB_LUT" ;
      return boost::shared_ptr< l1t::CaloParams >() ;
    }

    std::string hexData;
    hfThrLUT.fillVariable( "DATA",   hexData);
    std::vector<uint32_t> rawData;
    hexStringToVector(hexData, rawData);

    // Convert the LUT to offline format
    std::vector<int> hfThrVector;
    for (size_t i = 0; i < 4; i++) {
      hfThrVector.push_back(rawData[i]);
    }
    m_params_helper->setMinimumBiasThresholds(hfThrVector);
  } else {
    // No LUT to load
  }

  if (!centrality_lut_key.empty() && !ext_centrality_lut_key.empty()) {

    queryColumns.clear();
    queryColumns.push_back( "DATA" ) ;            // Hex LUT data
    l1t::OMDSReader::QueryResults centralityLUT = m_omdsReader.basicQuery( queryColumns,
									   stage1CaloSchema,
									   "CENTRALITY_LUT",
									   "CENTRALITY_LUT.CENTRALITY_LUT_KEY",
									   m_omdsReader.singleAttribute(centrality_lut_key)
      ) ;

    if( centralityLUT.queryFailed() || centralityLUT.numberRows() != 1){
      edm::LogError( "L1-O2O: L1TCaloParamsStage1HIOnlineProd" ) << "Cannot get CENTRALITY" ;
      return boost::shared_ptr< l1t::CaloParams >() ;
    }

    std::string hexData;
    centralityLUT.fillVariable( "DATA",   hexData);
    std::vector<uint32_t> rawData;
    L1TCaloParamsStage1HIOnlineProd::hexStringToVector(hexData, rawData);

    // Convert the LUT to offline format
    std::ostringstream oss;
    oss << "#<header> V2 5 14 </header>" << std::endl;
    for (size_t i = 0; i < rawData.size(); i++) {
      oss << i << " " << rawData[i] << std::endl;
    }

    queryColumns.clear();
    queryColumns.push_back( "DATA" ) ;            // Hex LUT data
    l1t::OMDSReader::QueryResults extCentLUT = m_omdsReader.basicQuery( queryColumns,
									stage1CaloSchema,
									"EXT_CENTRALITY_LUT",
									"EXT_CENTRALITY_LUT.EXT_CENTRALITY_LUT_KEY",
									m_omdsReader.singleAttribute(ext_centrality_lut_key)
      ) ;

    if( extCentLUT.queryFailed() || extCentLUT.numberRows() != 1){
      edm::LogError( "L1-O2O: L1TCaloParamsStage1HIOnlineProd" ) << "Cannot get EXT_CENTRALITY_LUT" ;
      return boost::shared_ptr< l1t::CaloParams >() ;
    }

    std::string hexData2;
    extCentLUT.fillVariable( "DATA",   hexData2);
    std::vector<uint32_t> rawData2;
    L1TCaloParamsStage1HIOnlineProd::hexStringToVector(hexData2, rawData2);

    // Convert the LUT to offline format
    for (size_t i = 0; i < rawData2.size(); i++) {
      oss << i+rawData.size() << " " << rawData2[i] << std::endl;
    }

    // Create the istream for l1t::LUT
    std::istringstream is(oss.str());
    l1t::LUT lut(is);
    m_params_helper->setCentralityLUT(lut);
  } else {
    // No LUT to load
  }

///  // fill in the defaults, the code is borrowed from L1Trigger/L1TCalorimeter/plugins/L1TCaloParamsESProducer.cc
///  if ((etSumEtaMin.size() == etSumEtaMax.size()) &&  (etSumEtaMin.size() == etSumEtThreshold.size())) {
///    for (unsigned i=0; i<etSumEtaMin.size(); ++i) {
///      m_params_helper->setEtSumEtaMin(i, etSumEtaMin.at(i));
///      m_params_helper->setEtSumEtaMax(i, etSumEtaMax.at(i));
///      m_params_helper->setEtSumEtThreshold(i, etSumEtThreshold.at(i));
///    }
///  }
///  else {
///    edm::LogError("l1t|calo") << "Inconsistent number of EtSum parameters" << std::endl;
///  }
///  m_params_helper->setTowerLsbSum(towerLsbSum);
///  m_params_helper->setRegionLsb  (regionLsb);
///  m_params_helper->setJetLsb     (jetLsb);
///  m_params_helper->setEgLsb      (egLsb);

  caloParams = static_cast< boost::shared_ptr<l1t::CaloParams> > (m_params_helper);
  return caloParams;
}

//define this as a plug-in
DEFINE_FWK_EVENTSETUP_MODULE(L1TCaloParamsStage1HIOnlineProd);
