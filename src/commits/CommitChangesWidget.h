#pragma once

/****************************************************************************************
 ** GitQlient is an application to manage and operate one or several Git repositories. With
 ** GitQlient you will be able to add commits, branches and manage all the options Git provides.
 ** Copyright (C) 2020  Francesc Martinez
 **
 ** LinkedIn: www.linkedin.com/in/cescmm/
 ** Web: www.francescmm.com
 **
 ** This program is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 2 of the License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this library; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***************************************************************************************/

#include <QWidget>
#include <QMap>

class QListWidget;
class QListWidgetItem;
class RevisionsCache;
class GitBase;
class RevisionFiles;

namespace Ui
{
class CommitChangesWidget;
}

class CommitChangesWidget : public QWidget
{
   Q_OBJECT

signals:
   void signalShowDiff(const QString &sha, const QString &parentSha, const QString &fileName);
   void signalChangesCommitted(bool commited);
   void signalCheckoutPerformed();
   void signalShowFileHistory(const QString &fileName);
   void signalUpdateWip();
   void signalCancelAmend(const QString &commitSha);

   /**
    * @brief signalEditFile Signal triggered when the user wants to edit a file and is running GitQlient from QtCreator.
    * @param fileName The file name
    * @param line The line
    * @param column The column
    */
   void signalEditFile(const QString &fileName, int line, int column);

public:
   explicit CommitChangesWidget(const QSharedPointer<RevisionsCache> &cache, const QSharedPointer<GitBase> &git,
                                QWidget *parent = nullptr);

   ~CommitChangesWidget();

   virtual void configure(const QString &sha) = 0;
   virtual void reload() final;
   virtual void clear() final;

protected:
   Ui::CommitChangesWidget *ui = nullptr;
   QSharedPointer<RevisionsCache> mCache;
   QSharedPointer<GitBase> mGit;
   QString mCurrentSha;
   QMap<QString, QPair<bool, QListWidgetItem *>> mCurrentFilesCache;

   virtual bool commitChanges() = 0;
   virtual void showUnstagedMenu(const QPoint &pos) = 0;

   virtual void insertFiles(const RevisionFiles &files, QListWidget *fileList) final;
   virtual void prepareCache() final;
   virtual void clearCache() final;
   virtual void addAllFilesToCommitList() final;
   virtual void requestDiff(const QString &fileName) final;
   virtual void addFileToCommitList(QListWidgetItem *item) final;
   virtual void revertAllChanges() final;
   virtual void removeFileFromCommitList(QListWidgetItem *item) final;
   virtual QStringList getFiles() final;
   virtual bool checkMsg(QString &msg) final;
   virtual void updateCounter(const QString &text) final;
   virtual bool hasConflicts() final;
   virtual void resetFile(QListWidgetItem *item) final;
   virtual QColor getColorForFile(const RevisionFiles &files, int index) const final;

   static QString lastMsgBeforeError;
   static const int kMaxTitleChars;
};