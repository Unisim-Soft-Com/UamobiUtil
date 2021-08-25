#pragma once
#include "submodules/UNAQtCommons/widgets/parents/inframedWidget.h"
#include "networking/things.h"


namespace independent_nodes
{
	enum nodelist
	{
		NotANode,
		PagedSearch,
		FilterSelect,
		DocResults,
		EditableDocResults,
		SubbranchingDocResults,
		ReceiptParameters,
		NormalScaning,
		PrintingScaning,
		MulticontrolScaning,
		IdDependentSelect,
		SelectItemFromList,
		PlaceSelect,
		InventoryParameters,
		ParentDocument,
		BarcodeFilterSelect,
		Subbranch,
		StaticSubbranch,
		Switch,
		Sender,
		ObservedScaning,
		ListPickObs,
		SkipNode,
		SwitchByScanBc,
		DefaultNode,
		IdDependentMultiselect,
		NormalScaningWithPrint,
		SelectItemByBarcode,
		IdDependentSelectByBarcode
	};
}

class BranchException;
class IndependentBranchNode : public inframedWidget
{
	Q_OBJECT	
protected:
	int id;
	int backtracksTo;
	
	using inframedWidget::_arrowReaction;
	using inframedWidget::_backReaction;
	using inframedWidget::_numberReaction;
	using inframedWidget::_captureNumbers;

	virtual void _handleRecord(RecEntity) = 0;
	virtual void _sendDataRequest() = 0;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) = 0;
	virtual void _rewriteNames(QStringList & names);
public:
	IndependentBranchNode(int Id, QWidget* parent = Q_NULLPTR) 
        : inframedWidget(parent), id(Id), backtracksTo(0) {}
    int getId() { return id; }
    int& backtracking() {	return backtracksTo;	}
	void processRecord(RecEntity);
	void loadData();
	void setOverloads(const QVector<QueryTemplates::OverloadableQuery>& oqs);
	void changeNames(QStringList& names);
    void throwException(BranchException*);
signals:
	void done(RecEntity);
    void exceptionThrown(BranchException*);
};
