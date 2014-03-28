#include <SnoopFlowChange>
#include <SnoopIp>
#include <SnoopTcp>
#include <SnoopUdp>

REGISTER_METACLASS(SnoopFlowChange, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopFlowChangeItem
// ----------------------------------------------------------------------------
SnoopFlowChangeItem::SnoopFlowChangeItem()
{
  enabled           = true;
  log               = true;
  protocol          = Tcp;

  srcIp             = 0;
  srcIpMask         = 0;
  srcIpChangeType   = IpCopy;
  srcIpFixValue     = 0;

  srcPort           = 0;
  srcPortChangeType = PortAutoInc;
  srcPortFixValue   = 0;

  dstIp             = 0;
  dstIpMask         = 0;
  dstIpChangeType   = IpFix;
  dstIpFixValue     = 0;

  dstPort           = 0;
  dstPortChangeType = PortCopy;
  dstPortFixValue   = 0;
}

bool SnoopFlowChangeItem::prepare(VError& error)
{
  if (srcIpChangeType == IpFix && srcIpFixValue == 0)
  {
    SET_ERROR(VError, "srcIpFixValue is zero", VERR_VALUE_IS_ZERO);
    return false;
  }
  if (srcPortChangeType == PortFix && srcPortFixValue == 0)
  {
    SET_ERROR(VError, "srcPortFixValue is zero", VERR_VALUE_IS_ZERO);
    return false;
  }
  if (dstIpChangeType == IpFix && dstIpFixValue == 0)
  {
    SET_ERROR(VError, "dstIpFixValue is zero", VERR_VALUE_IS_ZERO);
    return false;
  }
  if (dstPortChangeType == PortFix && dstPortFixValue == 0)
  {
    SET_ERROR(VError, "dstPortFixValue is zero", VERR_VALUE_IS_ZERO);
    return false;
  }
  return true;
}

bool SnoopFlowChangeItem::check(SnoopTransportFlowKey& flowKey, Protocol protocol)
{
  if (!this->enabled) return false;
  if (this->protocol != protocol)                             return false;
  if ((flowKey.srcIp & this->srcIpMask)     != this->srcIp)   return false;
  if (this->srcPort != 0 && flowKey.srcPort != this->srcPort) return false;
  if ((flowKey.dstIp & this->dstIpMask)     != this->dstIp)   return false;
  if (this->dstPort != 0 && flowKey.dstPort != this->dstPort) return false;
  return true;
}

void SnoopFlowChangeItem::load(VXml xml)
{
  enabled           = xml.getBool("enabled", enabled);
  log               = xml.getBool("log", log);
  protocol          = (Protocol)xml.getInt("protocol", (int)protocol);

  srcIp             = xml.getStr("srcIp", srcIp.str());
  srcIpMask         = xml.getStr("srcIpMask", srcIpMask.str());
  srcIpChangeType   = (IpChangeType)xml.getInt("srcIpChangeType", (int)srcIpChangeType);
  srcIpFixValue     = xml.getStr("srcIpFixValue", srcIpFixValue.str());

  srcPort           = (UINT16)xml.getInt("srcPort", (int)srcPort);
  srcPortChangeType = (PortChangeType)xml.getInt("srcPortChangeType", (int)srcPortChangeType);
  srcPortFixValue   = (UINT16)xml.getInt("srcPortFixValue", (int)srcPortFixValue);

  dstIp             = xml.getStr("dstIp", dstIp.str());
  dstIpMask         = xml.getStr("dstIpMask", dstIpMask.str());
  dstIpChangeType   = (IpChangeType)xml.getInt("dstIpChangeType", (int)dstIpChangeType);
  dstIpFixValue     = xml.getStr("dstIpFixValue", dstIpFixValue.str());

  dstPort           = (UINT16)xml.getInt("dstPort", (int)dstPort);
  dstPortChangeType = (PortChangeType)xml.getInt("dstPortChangeType", (int)dstPortChangeType);
  dstPortFixValue   = (UINT16)xml.getInt("dstPortFixValue", (int)dstPortFixValue);
}

void SnoopFlowChangeItem::save(VXml xml)
{
  xml.setBool("enabled", enabled);
  xml.setBool("log", log);
  xml.setInt("protocol", (int)protocol);

  xml.setStr("srcIp", srcIp.str());
  xml.setStr("srcIpMask", srcIpMask.str());
  xml.setInt("srcIpChangeType", (int)srcIpChangeType);
  xml.setStr("srcIpFixValue", srcIpFixValue.str());

  xml.setInt("srcPort", (int)srcPort);
  xml.setInt("srcPortChangeType", (int)srcPortChangeType);
  xml.setInt("srcPortFixValue", (int)srcPortFixValue);

  xml.setStr("dstIp", dstIp.str());
  xml.setStr("dstIpMask", dstIpMask.str());
  xml.setInt("dstIpChangeType", (int)dstIpChangeType);
  xml.setStr("dstIpFixValue", dstIpFixValue.str());

  xml.setInt("dstPort", (int)dstPort);
  xml.setInt("dstPortChangeType", (int)dstPortChangeType);
  xml.setInt("dstPortFixValue", (int)dstPortFixValue);
}

#ifdef QT_GUI_LIB
void SnoopFlowChangeItem::initialize(QTreeWidget* treeWidget)
{
  treeWidget->setMinimumWidth(1200);

  QStringList headerLables;
  headerLables << "Enabled" << "Log" << "Protocol"
    << "srcIp"   << "SrcIpMask"   << "SrcIpChangeType"   << "SrcIpFixValue"
    << "srcPort"                  << "SrcPortChangeType" << "SrcPortFixValue"
    << "dstIp"   << "dstIpMask"   << "dstIpChangeType"   << "dstIpFixValue"
    << "dstPort"                  << "dstPortChangeType" << "dstPortFixValue";
  treeWidget->setHeaderLabels(headerLables);

  treeWidget->setColumnWidth(ENABLED_IDX,          30);
  treeWidget->setColumnWidth(LOG_IDX,              30);
  treeWidget->setColumnWidth(PROTOCOL_IDX,         50);

  treeWidget->setColumnWidth(SRC_IP_IDX,           100);
  treeWidget->setColumnWidth(SRC_IP_MASK_IDX,      100);
  treeWidget->setColumnWidth(SRC_IP_CHANGE_TYPE,   50);
  treeWidget->setColumnWidth(SRC_IP_FIX_VALUE,     100);

  treeWidget->setColumnWidth(SRC_PORT_IDX,         50);
  treeWidget->setColumnWidth(SRC_PORT_CHANGE_TYPE, 50);
  treeWidget->setColumnWidth(SRC_PORT_FIX_VALUE,   50);

  treeWidget->setColumnWidth(DST_IP_IDX,           100);
  treeWidget->setColumnWidth(DST_IP_MASK_IDX,      100);
  treeWidget->setColumnWidth(DST_IP_CHANGE_TYPE,   50);
  treeWidget->setColumnWidth(DST_IP_FIX_VALUE,     100);

  treeWidget->setColumnWidth(DST_PORT_IDX,         50);
  treeWidget->setColumnWidth(DST_PORT_CHANGE_TYPE, 50);
  treeWidget->setColumnWidth(DST_PORT_FIX_VALUE,   50);
}

void operator << (QTreeWidgetItem& treeWidgetItem, SnoopFlowChangeItem& item)
{
  treeWidgetItem.setCheckState(SnoopFlowChangeItem::ENABLED_IDX, item.enabled ? Qt::Checked : Qt::Unchecked);
  treeWidgetItem.setCheckState(SnoopFlowChangeItem::LOG_IDX,     item.log ? Qt::Checked : Qt::Unchecked);

  QComboBox* protocolComboBox = new QComboBox(treeWidgetItem.treeWidget());
  QStringList sl; sl << "TCP" << "UDP";
  protocolComboBox->insertItems(0, sl);
  protocolComboBox->setCurrentIndex((int)item.protocol);
  treeWidgetItem.treeWidget()->setItemWidget(&treeWidgetItem, SnoopFlowChangeItem::PROTOCOL_IDX, protocolComboBox);

  treeWidgetItem.setText(SnoopFlowChangeItem::SRC_IP_IDX, item.srcIp.str());
  treeWidgetItem.setText(SnoopFlowChangeItem::SRC_IP_MASK_IDX, item.srcIpMask.str());
  QComboBox* srcIpComboBox = new QComboBox(treeWidgetItem.treeWidget());
  QStringList ipChangeTypeList; ipChangeTypeList << "Copy" << "Fix";
  srcIpComboBox->insertItems(0, ipChangeTypeList);
  srcIpComboBox->setCurrentIndex((int)item.srcIpChangeType);
  treeWidgetItem.treeWidget()->setItemWidget(&treeWidgetItem, SnoopFlowChangeItem::SRC_IP_CHANGE_TYPE, srcIpComboBox);
  treeWidgetItem.setText(SnoopFlowChangeItem::SRC_IP_FIX_VALUE, item.srcIpFixValue.str());

  treeWidgetItem.setText(SnoopFlowChangeItem::SRC_PORT_IDX, QString::number(item.srcPort));
  QComboBox* srcPortComboBox = new QComboBox(treeWidgetItem.treeWidget());
  QStringList portChangeTypeList; portChangeTypeList << "Copy" << "AutoInc" << "Fix";
  srcPortComboBox->insertItems(0, portChangeTypeList);
  srcPortComboBox->setCurrentIndex((int)item.srcPortChangeType);
  treeWidgetItem.treeWidget()->setItemWidget(&treeWidgetItem, SnoopFlowChangeItem::SRC_PORT_CHANGE_TYPE, srcPortComboBox);
  treeWidgetItem.setText(SnoopFlowChangeItem::SRC_PORT_FIX_VALUE, QString::number(item.srcPortFixValue));

  treeWidgetItem.setText(SnoopFlowChangeItem::DST_IP_IDX, item.dstIp.str());
  treeWidgetItem.setText(SnoopFlowChangeItem::DST_IP_MASK_IDX, item.dstIpMask.str());
  QComboBox* dstIpComboBox = new QComboBox(treeWidgetItem.treeWidget());
  dstIpComboBox->insertItems(0, ipChangeTypeList);
  dstIpComboBox->setCurrentIndex((int)item.dstIpChangeType);
  treeWidgetItem.treeWidget()->setItemWidget(&treeWidgetItem, SnoopFlowChangeItem::DST_IP_CHANGE_TYPE, dstIpComboBox);
  treeWidgetItem.setText(SnoopFlowChangeItem::DST_IP_FIX_VALUE, item.dstIpFixValue.str());

  treeWidgetItem.setText(SnoopFlowChangeItem::DST_PORT_IDX, QString::number(item.dstPort));
  QComboBox* dstPortComboBox = new QComboBox(treeWidgetItem.treeWidget());
  dstPortComboBox->insertItems(0, portChangeTypeList);
  dstPortComboBox->setCurrentIndex((int)item.dstPortChangeType);
  treeWidgetItem.treeWidget()->setItemWidget(&treeWidgetItem, SnoopFlowChangeItem::DST_PORT_CHANGE_TYPE, dstPortComboBox);
  treeWidgetItem.setText(SnoopFlowChangeItem::DST_PORT_FIX_VALUE, QString::number(item.dstPortFixValue));

  treeWidgetItem.setFlags(treeWidgetItem.flags() | Qt::ItemIsEditable);
}

void operator << (SnoopFlowChangeItem& item, QTreeWidgetItem& treeWidgetItem)
{
  item.enabled  = treeWidgetItem.checkState(SnoopFlowChangeItem::ENABLED_IDX) == Qt::Checked;
  item.log      = treeWidgetItem.checkState(SnoopFlowChangeItem::LOG_IDX)     == Qt::Checked;
  QComboBox* protocolComboBox = dynamic_cast<QComboBox*>(treeWidgetItem.treeWidget()->itemWidget(&treeWidgetItem, SnoopFlowChangeItem::PROTOCOL_IDX));
  item.protocol = (SnoopFlowChangeItem::Protocol)(protocolComboBox->currentIndex());

  item.srcIp = treeWidgetItem.text(SnoopFlowChangeItem::SRC_IP_IDX);
  item.srcIpMask = treeWidgetItem.text(SnoopFlowChangeItem::SRC_IP_MASK_IDX);
  QComboBox* srcIpComboBox = dynamic_cast<QComboBox*>(treeWidgetItem.treeWidget()->itemWidget(&treeWidgetItem, SnoopFlowChangeItem::SRC_IP_CHANGE_TYPE));
  item.srcIpChangeType = (SnoopFlowChangeItem::IpChangeType)(srcIpComboBox->currentIndex());
  item.srcIpFixValue = treeWidgetItem.text(SnoopFlowChangeItem::SRC_IP_FIX_VALUE);

  item.srcPort = treeWidgetItem.text(SnoopFlowChangeItem::SRC_PORT_IDX).toUShort();
  QComboBox* srcPortComboBox = dynamic_cast<QComboBox*>(treeWidgetItem.treeWidget()->itemWidget(&treeWidgetItem, SnoopFlowChangeItem::SRC_PORT_CHANGE_TYPE));
  item.srcPortChangeType = (SnoopFlowChangeItem::PortChangeType)(srcPortComboBox->currentIndex());
  item.srcPortFixValue = treeWidgetItem.text(SnoopFlowChangeItem::SRC_PORT_FIX_VALUE).toUShort();

  item.dstIp = treeWidgetItem.text(SnoopFlowChangeItem::DST_IP_IDX);
  item.dstIpMask = treeWidgetItem.text(SnoopFlowChangeItem::DST_IP_MASK_IDX);
  QComboBox* dstIpComboBox = dynamic_cast<QComboBox*>(treeWidgetItem.treeWidget()->itemWidget(&treeWidgetItem, SnoopFlowChangeItem::DST_IP_CHANGE_TYPE));
  item.dstIpChangeType = (SnoopFlowChangeItem::IpChangeType)(dstIpComboBox->currentIndex());
  item.dstIpFixValue = treeWidgetItem.text(SnoopFlowChangeItem::DST_IP_FIX_VALUE);

  item.dstPort = treeWidgetItem.text(SnoopFlowChangeItem::DST_PORT_IDX).toUShort();
  QComboBox* dstPortComboBox = dynamic_cast<QComboBox*>(treeWidgetItem.treeWidget()->itemWidget(&treeWidgetItem, SnoopFlowChangeItem::DST_PORT_CHANGE_TYPE));
  item.dstPortChangeType = (SnoopFlowChangeItem::PortChangeType)(dstPortComboBox->currentIndex());
  item.dstPortFixValue = treeWidgetItem.text(SnoopFlowChangeItem::DST_PORT_FIX_VALUE).toUShort();

  VError error; item.prepare(error);
}
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopFlowChangeItems
// ----------------------------------------------------------------------------
SnoopFlowChangeItems::SnoopFlowChangeItems()
{
  VError error; prepare(error);
}

SnoopFlowChangeItems::~SnoopFlowChangeItems()
{

}

bool SnoopFlowChangeItems::prepare(VError& error)
{
  lastAutoIncSrcPort = 1024;
  lastAutoIncDstPort = 1024;

  for (int i = 0; i < count(); i++)
  {
    SnoopFlowChangeItem& item = (SnoopFlowChangeItem&)at(i);
    if (!item.prepare(error)) return false;
  }
  return true;
}

SnoopFlowChangeItem* SnoopFlowChangeItems::find(SnoopTransportFlowKey& flowKey, SnoopFlowChangeItem::Protocol protocol)
{
  for (int i = 0; i < count(); i++)
  {
    SnoopFlowChangeItem& item = (SnoopFlowChangeItem&)at(i);
    if (item.check(flowKey, protocol)) return &item;
  }
  return NULL;
}

SnoopTransportFlowKey SnoopFlowChangeItems::change(SnoopFlowChangeItem& item, SnoopTransportFlowKey& flowKey)
{
  SnoopTransportFlowKey res;

  switch (item.srcIpChangeType)
  {
    case SnoopFlowChangeItem::IpCopy:
      res.srcIp = flowKey.srcIp;
      break;
    case SnoopFlowChangeItem::IpFix:
      res.srcIp = item.srcIpFixValue;
      break;
  }

  switch (item.srcPortChangeType)
  {
    case SnoopFlowChangeItem::PortCopy:
      res.srcPort = flowKey.srcPort;
      break;
    case SnoopFlowChangeItem::PortAutoInc:
      res.srcPort = lastAutoIncSrcPort;
      lastAutoIncSrcPort++;
      break;
    case SnoopFlowChangeItem::PortFix:
      res.srcPort = item.srcPortFixValue;
      break;
  }

  switch (item.dstIpChangeType)
  {
    case SnoopFlowChangeItem::IpCopy:
      res.dstIp = flowKey.dstIp;
      break;
     case SnoopFlowChangeItem::IpFix:
      res.dstIp = item.dstIpFixValue;
      break;
  }

  switch (item.dstPortChangeType)
  {
    case SnoopFlowChangeItem::PortCopy:
      res.dstPort = flowKey.dstPort;
      break;
    case SnoopFlowChangeItem::PortAutoInc:
      res.dstPort = lastAutoIncDstPort;
      lastAutoIncDstPort++;
      break;
    case SnoopFlowChangeItem::PortFix:
      res.dstPort = item.dstPortFixValue;
      break;
  }

  return res;
}

void SnoopFlowChangeItems::load(VXml xml)
{
  clear();
  {
    xml_foreach (childXml, xml.childs())
    {
      SnoopFlowChangeItem item;
      item.load(childXml);
      push_back(item);
    }
  }
}

void SnoopFlowChangeItems::save(VXml xml)
{
  xml.clearChild();
  for (SnoopFlowChangeItems::iterator it = begin(); it != end(); it++)
  {
    SnoopFlowChangeItem& item = *it;
    VXml childXml = xml.addChild("item");
    item.save(childXml);
  }
}

#ifdef QT_GUI_LIB
#include "ui_vlistwidget.h"
void SnoopFlowChangeItems::optionAddWidget(QLayout* layout)
{
  VListWidget* widget = new VListWidget(layout->parentWidget(), this);
  SnoopFlowChangeItem::initialize(widget->ui->treeWidget);
  widget->setObjectName("snoopFlowChangeWidget");
  widget->itemsIntoTreeWidget();
  layout->addWidget(widget);
}

void SnoopFlowChangeItems::optionSaveDlg(QDialog* dialog)
{
  VListWidget* widget = dialog->findChild<VListWidget*>("snoopFlowChangeWidget");
  LOG_ASSERT(widget != NULL);
  widget->treeWidgetIntoItems();
}
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopFlowChange
// ----------------------------------------------------------------------------
SnoopFlowChange::SnoopFlowChange(void* owner) : SnoopProcess(owner)
{
  inCapture        = NULL;
  inFlowMgr        = NULL;
  outCapture       = NULL;
  outFlowMgr       = NULL;
  tcpChange        = true;
  udpChange        = true;
  changeItems.clear();
  inTcpFlowOffset  = 0;
  outTcpFlowOffset = 0;
  inUdpFlowOffset  = 0;
  outUdpFlowOffset = 0;
  tcpInOutMap.clear();
  udpInOutMap.clear();
}

SnoopFlowChange::~SnoopFlowChange()
{
  close();
}

bool SnoopFlowChange::doOpen()
{
  if (inCapture == NULL)
  {
    SET_ERROR(SnoopError, "inCapture is null", VERR_OBJECT_IS_NULL);
    return false;
  }
  if (inFlowMgr == NULL)
  {
    SET_ERROR(SnoopError, "inFlowMgr is null", VERR_OBJECT_IS_NULL);
    return false;
  }
  if (outCapture == NULL)
  {
    SET_ERROR(SnoopError, "outCapture is null", VERR_OBJECT_IS_NULL);
    return false;
  }
  if (outFlowMgr == NULL)
  {
    SET_ERROR(SnoopError, "outFlowMgr is null", VERR_OBJECT_IS_NULL);
    return false;
  }

  if (!changeItems.prepare(error)) return false;

  if (tcpChange)
  {
    inTcpFlowOffset = inFlowMgr->requestMemory_TcpFlow("SnoopFlowChangeIn", sizeof(SnoopFlowChangeFlowItem));
    inFlowMgr->connect(SIGNAL(__tcpFlowCreated(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__inTcpFlowCreate(SnoopTcpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
    inFlowMgr->connect(SIGNAL(__tcpFlowDeleted(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__inTcpFlowDelete(SnoopTcpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);

    outTcpFlowOffset = outFlowMgr->requestMemory_TcpFlow("SnoopFlowChangeOut", sizeof(SnoopFlowChangeFlowItem));
    outFlowMgr->connect(SIGNAL(__tcpFlowCreated(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__outTcpFlowCreate(SnoopTcpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
    outFlowMgr->connect(SIGNAL(__tcpFlowDeleted(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__outTcpFlowDelete(SnoopTcpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
  }

  if (udpChange)
  {
    inUdpFlowOffset = inFlowMgr->requestMemory_UdpFlow("SnoopFlowChangeIn", sizeof(SnoopFlowChangeFlowItem));
    inFlowMgr->connect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__inUdpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
    inFlowMgr->connect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__inUdpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);

    inUdpFlowOffset = outFlowMgr->requestMemory_UdpFlow("SnoopFlowChangeIn", sizeof(SnoopFlowChangeFlowItem));
    outFlowMgr->connect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__outUdpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
    outFlowMgr->connect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__outUdpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
  }

  tcpInOutMap.clear();
  udpInOutMap.clear();

  return SnoopProcess::doOpen();
}

bool SnoopFlowChange::doClose()
{
  if (inFlowMgr == NULL)
  {
    SET_ERROR(SnoopError, "inFlowMgr is null", VERR_OBJECT_IS_NULL);
    return true;
  }
  if (outFlowMgr == NULL)
  {
    SET_ERROR(SnoopError, "outFlowMgr is null", VERR_OBJECT_IS_NULL);
    return true;
  }

  if (tcpChange)
  {
    inFlowMgr->disconnect(SIGNAL(__tcpFlowCreated(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__inTcpFlowCreate(SnoopTcpFlowKey*,SnoopFlowValue*)));
    inFlowMgr->disconnect(SIGNAL(__tcpFlowDeleted(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__inTcpFlowDelete(SnoopTcpFlowKey*,SnoopFlowValue*)));

    outFlowMgr->disconnect(SIGNAL(__tcpFlowCreated(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__outTcpFlowCreate(SnoopTcpFlowKey*,SnoopFlowValue*)));
    outFlowMgr->disconnect(SIGNAL(__tcpFlowDeleted(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__outTcpFlowDelete(SnoopTcpFlowKey*,SnoopFlowValue*)));
  }

  if (udpChange)
  {
    inFlowMgr->disconnect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__inUdpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)));
    inFlowMgr->disconnect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__inUdpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)));

    outFlowMgr->disconnect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__outUdpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)));
    outFlowMgr->disconnect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__outUdpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)));
  }

  return SnoopProcess::doClose();
}

void SnoopFlowChange::_changeTcpFlow(SnoopPacket* packet, SnoopFlowChangeFlowItem* flowItem)
{
  Ip     oldSrcIp   = ntohl(packet->ipHdr->ip_src);
  UINT16 oldSrcPort = ntohs(packet->tcpHdr->th_sport);
  Ip     oldDstIp   = ntohl(packet->ipHdr->ip_dst);
  UINT16 oldDstPort = ntohs(packet->tcpHdr->th_dport);

  Ip     newSrcIp   = flowItem->to.srcIp;
  UINT16 newSrcPort = flowItem->to.srcPort;
  Ip     newDstIp   = flowItem->to.dstIp;
  UINT16 newDstPort = flowItem->to.dstPort;

  packet->ipHdr->ip_src    = htonl(newSrcIp);
  packet->tcpHdr->th_sport = htons(newSrcPort);
  packet->ipHdr->ip_dst    = htonl(newDstIp);
  packet->tcpHdr->th_dport = htons(newDstPort);

  UINT16 oldIpChecksum  = ntohs(packet->ipHdr->ip_sum);
  UINT16 oldTcpChecksum = ntohs(packet->tcpHdr->th_sum);

  UINT16 newIpChecksum;
  newIpChecksum = SnoopIp::recalculateChecksum(oldIpChecksum, oldSrcIp, newSrcIp);
  newIpChecksum = SnoopIp::recalculateChecksum(newIpChecksum, oldDstIp, newDstIp);
  // UINT16 correctIpChecksum = SnoopIp::checksum(packet->ipHdr); // gilgil temp 2014.03.25
  // LOG_DEBUG("ip checksum=0x%x 0x%x 0x%x", oldIpChecksum, newIpChecksum, correctIpChecksum); // gilgil temp 2014.03.25
  packet->ipHdr->ip_sum = htons(newIpChecksum);

  UINT16 newTcpChecksum;
  newTcpChecksum = SnoopIp::recalculateChecksum(oldTcpChecksum, oldSrcIp,   newSrcIp);
  newTcpChecksum = SnoopIp::recalculateChecksum(newTcpChecksum, oldDstIp,   newDstIp);
  newTcpChecksum = SnoopIp::recalculateChecksum(newTcpChecksum, oldSrcPort, newSrcPort);
  newTcpChecksum = SnoopIp::recalculateChecksum(newTcpChecksum, oldDstPort, newDstPort);
  // UINT16 correctTcpChecksum = SnoopTcp::checksum(packet->ipHdr, packet->tcpHdr);  // gilgil temp 2014.03.25
  // LOG_DEBUG("tcp checksum=0x%x 0x%x 0x%x", oldTcpChecksum, newTcpChecksum, correctTcpChecksum); // gilgil temp 2014.03.25
  packet->tcpHdr->th_sum = htons(newTcpChecksum);

  //LOG_DEBUG("change %s:%d > %s:%d into %s:%d > %s:%d",
  //  qPrintable(oldSrcIp.str()), oldSrcPort, qPrintable(oldDstIp.str()), oldDstPort,
  //  qPrintable(newSrcIp.str()), newSrcPort, qPrintable(newDstIp.str()), newDstPort); // gilgil temp 2014.03.26
}

void SnoopFlowChange::_changeUdpFlow(SnoopPacket* packet, SnoopFlowChangeFlowItem* flowItem)
{
  Ip     oldSrcIp   = ntohl(packet->ipHdr->ip_src);
  UINT16 oldSrcPort = ntohs(packet->udpHdr->uh_sport);
  Ip     oldDstIp   = ntohl(packet->ipHdr->ip_dst);
  UINT16 oldDstPort = ntohs(packet->udpHdr->uh_dport);

  Ip     newSrcIp   = flowItem->to.srcIp;
  UINT16 newSrcPort = flowItem->to.srcPort;
  Ip     newDstIp   = flowItem->to.dstIp;
  UINT16 newDstPort = flowItem->to.dstPort;

  packet->ipHdr->ip_src    = htonl(newSrcIp);
  packet->udpHdr->uh_sport = htons(newSrcPort);
  packet->ipHdr->ip_dst    = htonl(newDstIp);
  packet->udpHdr->uh_dport = htons(newDstPort);

  UINT16 oldIpChecksum  = ntohs(packet->ipHdr->ip_sum);
  UINT16 oldUdpChecksum = ntohs(packet->udpHdr->uh_sum);

  UINT16 newIpChecksum;
  newIpChecksum = SnoopIp::recalculateChecksum(oldIpChecksum, oldSrcIp, newSrcIp);
  newIpChecksum = SnoopIp::recalculateChecksum(newIpChecksum, oldDstIp, newDstIp);
  // UINT16 correctIpChecksum = SnoopIp::checksum(packet->ipHdr); // gilgil temp 2014.03.25
  // LOG_DEBUG("ip checksum=0x%x 0x%x 0x%x", oldIpChecksum, newIpChecksum, correctIpChecksum); // gilgil temp 2014.03.25
  packet->ipHdr->ip_sum = htons(newIpChecksum);

  UINT16 newUdpChecksum;
  newUdpChecksum = SnoopIp::recalculateChecksum(oldUdpChecksum, oldSrcIp,   newSrcIp);
  newUdpChecksum = SnoopIp::recalculateChecksum(newUdpChecksum, oldDstIp,   newDstIp);
  newUdpChecksum = SnoopIp::recalculateChecksum(newUdpChecksum, oldSrcPort, newSrcPort);
  newUdpChecksum = SnoopIp::recalculateChecksum(newUdpChecksum, oldDstPort, newDstPort);
  // UINT16 correctTcpChecksum = SnoopTcp::checksum(packet->ipHdr, packet->tcpHdr);  // gilgil temp 2014.03.25
  // LOG_DEBUG("tcp checksum=0x%x 0x%x 0x%x", oldTcpChecksum, newTcpChecksum, correctTcpChecksum); // gilgil temp 2014.03.25
  packet->udpHdr->uh_sum = htons(newUdpChecksum);

  //LOG_DEBUG("change %s:%d > %s:%d into %s:%d > %s:%d",
  //  qPrintable(oldSrcIp.str()), oldSrcPort, qPrintable(oldDstIp.str()), oldDstPort,
  //  qPrintable(newSrcIp.str()), newSrcPort, qPrintable(newDstIp.str()), newDstPort); // gilgil temp 2014.03.26
}

void SnoopFlowChange::processInOut(SnoopPacket* packet)
{
  if (packet->ipHdr == NULL)
  {
    emit unchanged(packet);
    return;
  }

  bool _changed = false;
  if (packet->tcpHdr != NULL)
  {
    if (tcpChange)
    {
      SnoopFlowChangeFlowItem* flowItem = (SnoopFlowChangeFlowItem*)(packet->flowValue->totalMem + inTcpFlowOffset);
      if (flowItem->changed)
      {
        _changeTcpFlow(packet, flowItem);
        _changed = true;
      }
    }
  } else
  if (packet->udpHdr != NULL)
  {
    if (udpChange)
    {
      SnoopFlowChangeFlowItem* flowItem = (SnoopFlowChangeFlowItem*)(packet->flowValue->totalMem + inUdpFlowOffset);
      if (flowItem->changed)
      {
        _changeUdpFlow(packet, flowItem);
        _changed = true;
      }
    }
  }

  if (_changed)
  {
    emit changed(packet);
  } else
  {
    emit unchanged(packet);
  }
}

void SnoopFlowChange::processOutIn(SnoopPacket* packet)
{
  if (packet->ipHdr == NULL)
  {
    emit unchanged(packet);
    return;
  }

  bool _changed = false;
  if (packet->tcpHdr != NULL)
  {
    if (tcpChange)
    {
      SnoopFlowChangeFlowItem* flowItem = (SnoopFlowChangeFlowItem*)(packet->flowValue->totalMem + outTcpFlowOffset);
      if (flowItem->changed)
      {
        _changeTcpFlow(packet, flowItem);
        _changed = true;
      }
    }
  } else
  if (packet->udpHdr != NULL)
  {
    if (udpChange)
    {
      SnoopFlowChangeFlowItem* flowItem = (SnoopFlowChangeFlowItem*)(packet->flowValue->totalMem + outUdpFlowOffset);
      if (flowItem->changed)
      {
        _changeUdpFlow(packet, flowItem);
        _changed = true;
      }
    }
  }

  if (_changed)
  {
    emit changed(packet);
  } else
  {
    emit unchanged(packet);
  }
}
void SnoopFlowChange::__inTcpFlowCreate(SnoopTcpFlowKey* key, SnoopFlowValue* value)
{
  LOG_DEBUG("%s:%d > %s:%d", qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort); // gilgil temp 2014.03.26
  SnoopFlowChangeFlowItem* flowItem = (SnoopFlowChangeFlowItem*)(value->totalMem + inTcpFlowOffset);

  SnoopFlowChangeItem* item = changeItems.find(*key, SnoopFlowChangeItem::Tcp);
  if (item != NULL)
  {
    flowItem->changed = true;
    flowItem->from    = *key;
    flowItem->to      = changeItems.change(*item, *key);;
    if (item->log)
    {
      LOG_INFO("change %s:%d > %s:%d into %s:%d > %s:%d",
        qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort,
        qPrintable(flowItem->to.srcIp.str()), flowItem->to.srcPort, qPrintable(flowItem->to.dstIp.str()), flowItem->to.dstPort);
    }

    VLock lock(tcpInOutMap);
    tcpInOutMap[flowItem->to] = flowItem->from;
    return;
  }

  flowItem->changed = false;
  flowItem->from    = *key;
  flowItem->to      = *key;
}

void SnoopFlowChange::__inTcpFlowDelete(SnoopTcpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(key)
  Q_UNUSED(value)
}

void SnoopFlowChange::__outTcpFlowCreate(SnoopTcpFlowKey* key, SnoopFlowValue* value)
{
  LOG_DEBUG("%s:%d > %s:%d", qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort); // gilgil temp 2014.03.26
  SnoopFlowChangeFlowItem* flowItem = (SnoopFlowChangeFlowItem*)(value->totalMem + outTcpFlowOffset);

  SnoopTcpFlowKey rkey = key->reverse();

  VLock lock(tcpInOutMap);
  SnoopFlowChangeMap::iterator it = tcpInOutMap.find(rkey);
  if (it != tcpInOutMap.end())
  {
    SnoopTransportFlowKey rvalue = it.value();
    flowItem->changed = true;
    flowItem->from    = *key;
    flowItem->to.srcIp   = rvalue.dstIp;
    flowItem->to.srcPort = rvalue.dstPort;
    flowItem->to.dstIp   = rvalue.srcIp;
    flowItem->to.dstPort = rvalue.srcPort;

    LOG_DEBUG("change %s:%d > %s:%d into %s:%d > %s:%d",
      qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort,
      qPrintable(flowItem->to.srcIp.str()), flowItem->to.srcPort, qPrintable(flowItem->to.dstIp.str()), flowItem->to.dstPort);

    return;
  }

  flowItem->changed = false;
  flowItem->from    = *key;
  flowItem->to      = *key;
}

void SnoopFlowChange::__outTcpFlowDelete(SnoopTcpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(key)
  Q_UNUSED(value)
}

void SnoopFlowChange::__inUdpFlowCreate(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{
  LOG_DEBUG("%s:%d > %s:%d", qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort); // gilgil temp 2014.03.26
  SnoopFlowChangeFlowItem* flowItem = (SnoopFlowChangeFlowItem*)(value->totalMem + inUdpFlowOffset);

  SnoopFlowChangeItem* item = changeItems.find(*key, SnoopFlowChangeItem::Udp);
  if (item != NULL)
  {
    flowItem->changed = true;
    flowItem->from    = *key;
    flowItem->to      = changeItems.change(*item, *key);;
    if (item->log)
    {
      LOG_INFO("change %s:%d > %s:%d into %s:%d > %s:%d",
        qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort,
        qPrintable(flowItem->to.srcIp.str()), flowItem->to.srcPort, qPrintable(flowItem->to.dstIp.str()), flowItem->to.dstPort);
    }

    VLock lock(udpInOutMap);
    udpInOutMap[flowItem->to] = flowItem->from;
    return;
  }

  flowItem->changed = false;
  flowItem->from    = *key;
  flowItem->to      = *key;
}

void SnoopFlowChange::__inUdpFlowDelete(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(*key)
  Q_UNUSED(*value)
}

void SnoopFlowChange::__outUdpFlowCreate(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{
  LOG_DEBUG("%s:%d > %s:%d", qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort); // gilgil temp 2014.03.26
  SnoopFlowChangeFlowItem* flowItem = (SnoopFlowChangeFlowItem*)(value->totalMem + outUdpFlowOffset);

  SnoopTcpFlowKey rkey = key->reverse();

  VLock lock(udpInOutMap);
  SnoopFlowChangeMap::iterator it = udpInOutMap.find(rkey);
  if (it != udpInOutMap.end())
  {
    SnoopTransportFlowKey rvalue = it.value();
    flowItem->changed = true;
    flowItem->from    = *key;
    flowItem->to.srcIp   = rvalue.dstIp;
    flowItem->to.srcPort = rvalue.dstPort;
    flowItem->to.dstIp   = rvalue.srcIp;
    flowItem->to.dstPort = rvalue.srcPort;

    LOG_DEBUG("change %s:%d > %s:%d into %s:%d > %s:%d",
      qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort,
      qPrintable(flowItem->to.srcIp.str()), flowItem->to.srcPort, qPrintable(flowItem->to.dstIp.str()), flowItem->to.dstPort);

    return;
  }

  flowItem->changed = false;
  flowItem->from    = *key;
  flowItem->to      = *key;
}

void SnoopFlowChange::__outUdpFlowDelete(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(*key)
  Q_UNUSED(*value)
}

void SnoopFlowChange::load(VXml xml)
{
  SnoopProcess::load(xml);

  QString inCaptureName = xml.getStr("inCapture", "");
  if (inCaptureName != "") inCapture = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(inCaptureName));
  QString inFlowMgrName = xml.getStr("inFlowMgr", "");
  if (inFlowMgrName != "") inFlowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(inFlowMgrName));
  QString outCaptureName = xml.getStr("outCapture", "");
  if (outCaptureName != "") outCapture = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(outCaptureName));
  QString outFlowMgrName = xml.getStr("outFlowMgr", "");
  if (outFlowMgrName != "") outFlowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(outFlowMgrName));
  tcpChange = xml.getBool("tcpChange", tcpChange);
  udpChange = xml.getBool("udpChange", udpChange);
  changeItems.load(xml.gotoChild("changeItems"));
}

void SnoopFlowChange::save(VXml xml)
{
  SnoopProcess::save(xml);

  QString inCaptureName = inCapture == NULL ? "" : inCapture->name;
  xml.setStr("inCapture", inCaptureName);
  QString inFlowMgrName = inFlowMgr == NULL ? "" : inFlowMgr->name;
  xml.setStr("inFlowMgr", inFlowMgrName);
  QString outCaptureName = outCapture == NULL ? "" : outCapture->name;
  xml.setStr("outCapture", outCaptureName);
  QString outFlowMgrName = outFlowMgr == NULL ? "" : outFlowMgr->name;
  xml.setStr("outFlowMgr", outFlowMgrName);
  xml.setBool("tcpChange", tcpChange);
  xml.setBool("udpChange", udpChange);
  changeItems.save(xml.gotoChild("changeItems"));
}

#ifdef QT_GUI_LIB
void SnoopFlowChange::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  QStringList captureList = ((VGraph*)owner)->objectList.findNamesByCategoryName("SnoopCapture");
  QStringList flowMgrList = ((VGraph*)owner)->objectList.findNamesByClassName("SnoopFlowMgr");

  VOptionable::addComboBox(layout, "cbxInCapture", "In Capture", captureList, -1, inCapture == NULL ? "" : inCapture->name);
  VOptionable::addComboBox(layout, "cbxInFlowMgr", "In FlowMgr", flowMgrList, -1, inFlowMgr == NULL ? "" : inFlowMgr->name);
  VOptionable::addComboBox(layout, "cbxOutCapture", "Out Capture", captureList, -1, outCapture == NULL ? "" : outCapture->name);
  VOptionable::addComboBox(layout, "cbxOutFlowMgr", "Out FlowMgr", flowMgrList, -1, outFlowMgr == NULL ? "" : outFlowMgr->name);
  VOptionable::addCheckBox(layout, "chkTcpChange", "TCP Change", tcpChange);
  VOptionable::addCheckBox(layout, "chkUdpChange", "UDP Change", udpChange);
  changeItems.optionAddWidget(layout);
}

void SnoopFlowChange::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  inCapture = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxInCapture")->currentText()));
  inFlowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxInFlowMgr")->currentText()));
  outCapture = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxOutCapture")->currentText()));
  outFlowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxOutFlowMgr")->currentText()));
  tcpChange = dialog->findChild<QCheckBox*>("chkTcpChange")->checkState() == Qt::Checked;
  udpChange = dialog->findChild<QCheckBox*>("chkUdpChange")->checkState() == Qt::Checked;
  changeItems.optionSaveDlg(dialog);
}
#endif // QT_GUI_LIB
