#include <SnoopProcessFactory>

#include <SnoopBlock>
#include <SnoopDataChange>
#include <SnoopDelay>
#include <SnoopFlowMgr>
#include <SnoopFlowMgrTest>
#include <SnoopDump>
#include <SnoopTcpBlock>
#include <SnoopWriteAdapter>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopProcessFactory
// ----------------------------------------------------------------------------
void SnoopProcessFactory::explicitLink()
{
  SnoopBlock        block;
  SnoopDataChange   dataChange;
  SnoopDelay        delay;
  SnoopDump         dump;
  SnoopFlowMgr      flowMgr;
  SnoopFlowMgrTest  flowMgrTest;
  SnoopTcpBlock     tcpBlock;
  SnoopWriteAdapter writeAdapter;
}

SnoopProcess* SnoopProcessFactory::createDefaultProcess()
{
  // ----- gilgil temp 2012.06.08 -----
  // return new SnoopProcessLog;
  // ----------------------------------
  return NULL;
}
